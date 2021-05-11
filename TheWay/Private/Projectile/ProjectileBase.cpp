// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/ProjectileBase.h"
#include "Components/WeaponStatComponent.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utility/CreataekUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Character/BaseCharacter.h"
#include "TheWay.h"
#include "GenericTeamAgentInterface.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	Body = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	RootComponent = Body;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Body);
	Mesh->bOnlyOwnerSee = false;
	Mesh->bOwnerNoSee = false;
	Mesh->bCastDynamicShadow = false;
	Mesh->bReceivesDecals = false;
	Mesh->AlwaysLoadOnClient = true;
	Mesh->AlwaysLoadOnServer = true;
	Mesh->bTraceComplexOnMove = true;
	//Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionObjectType(ECC_NEUTRAL_OVERLAP);
	/*Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);*/

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->InitialSpeed = 2000.0f;
	Movement->MaxSpeed = 10000.0f;
	Movement->bRotationFollowsVelocity = true;
	Movement->bIsHomingProjectile = false;
	Movement->HomingAccelerationMagnitude = 1000.0f;
	//Movement->ProjectileGravityScale = Gravity;
}

void AProjectileBase::Shoot(FVector Direction, float InitialSpeed)
{
	ShotDirection = Direction.GetSafeNormal();

	if (Movement)
	{
		Movement->Velocity = ShotDirection * (Movement->InitialSpeed + InitialSpeed);
	}

	if (ProjectileLifeSpan > 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(LifeSpanTimer);
		GetWorld()->GetTimerManager().SetTimer(LifeSpanTimer, this, &AProjectileBase::TimerHandle_LifeTime, ProjectileLifeSpan, false);
	}

	if (Movement->bIsHomingProjectile)
	{
		GetWorld()->GetTimerManager().ClearTimer(SearchHomingTargetTimer);
		GetWorld()->GetTimerManager().SetTimer(SearchHomingTargetTimer, this, &AProjectileBase::TimerHandle_SearchHomingTarget, SearchHomingTargetInterval, true);
	}
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (auto TeamAgentInterface = Cast<IGenericTeamAgentInterface>(GetInstigator()))
	{
		SetGenericTeamId(TeamAgentInterface->GetGenericTeamId());
	}
}

void AProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Movement->OnProjectileStop.AddDynamic(this, &AProjectileBase::OnCollided);
	Movement->OnProjectileBounce.AddDynamic(this, &AProjectileBase::OnBounce);
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AProjectileBase::GetFinalAttackDamage() const
{
	ABaseCharacter* instigatedCharacter = Cast<ABaseCharacter>(GetInstigator());
	float FinalDamage = AttackDamage + (instigatedCharacter ? instigatedCharacter->GetFinalAttckDamage() : 0.0f);
	return FinalDamage;
}

void AProjectileBase::OnCollided(const FHitResult& HitResult)
{
	if (HasAuthority())
	{
		MakeNoise(1.0f, GetInstigator());

		if (HitParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f), true);
		}

		if (DamageRadius == 0.0f)
		{
			if (ABaseCharacter* Character = Cast<ABaseCharacter>(HitResult.GetActor()))
			{
				FPointDamageEvent DamageEvent;
				QueryPointDamageEvent(GetFinalAttackDamage(), HitResult, DamageEvent);
				Character->TakeDamage(GetFinalAttackDamage(), DamageEvent, GetInstigator()->Controller, this);
				Character->GetCharacterMovement()->AddImpulse(DamageEvent.ShotDirection * KnockbackPower, true);

				if (ExplosionSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation(), GetActorRotation());
				}
			}
		}
		else
		{
			Explode(GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
		}

		Destroy();
	}
}

void AProjectileBase::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	if (HasAuthority())
	{
		MakeNoise(1.0f, GetInstigator());

		if (HitParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f), true);
		}
		SetLifeSpan(0.2f);
	}
}

void AProjectileBase::Explode(const FVector& ImpactPosition, const FRotator& ImpactRotation)
{
	ReturnIfFalse(DamageRadius > 0.0f);
	ReturnIfFalse(GetInstigator());
	ReturnIfFalse(HasAuthority());

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation(), GetActorRotation());
	}

	TMap<AActor*, FRadialDamageEvent> OverlapInfoMap;
	TArray<AActor*> IgnoreActors; 
	IgnoreActors.Add(this);

	QueryRadialDamageEvent(GetFinalAttackDamage(), ImpactPosition, ImpactRotation, DamageRadius, IgnoreActors, ECCOverlapObjectType, OverlapInfoMap);

	for (auto& OverlapInfo : OverlapInfoMap)
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(OverlapInfo.Key))
		{
			Character->TakeDamage(GetFinalAttackDamage(), OverlapInfo.Value, GetInstigator()->Controller, this);

			FVector LaunchVelocity = (Character->GetActorLocation() - ImpactPosition).GetSafeNormal();
			LaunchVelocity.X += KnockbackPower;
			LaunchVelocity.Y += KnockbackPower;

			Character->LaunchCharacter(LaunchVelocity, false, false);
		}
		/*if (ABaseCharacter* Character = Cast<ABaseCharacter>(HitResult.GetActor()))
		{
		}*/
	}
	//UGameplayStatics::ApplyRadialDamage(this, GetFinalAttackDamage(), ImpactPosition, DamageRadius, UDamageType::StaticClass(), IgnoreActors, this, GetInstigator()->GetController());

	/*static TArray<FActorDamageEvent> Events{ FActorDamageEvent() };

	FHitResult HitResult;
	FPointDamageEvent DamageEvent;
	QueryPointDamageEvent(Damage, HitResult, DamageEvent);
	Events[0] = { HitResult.GetActor(), &DamageEvent };

	HandleDamageAction(Events, this);*/
}

void AProjectileBase::TimerHandle_LifeTime()
{
	Explode(GetActorLocation(), FRotator());

	//SetLifeSpan(0.2f);
	Destroy();

	if (NoHitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NoHitParticle, GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f), true);
	}
}

void AProjectileBase::TimerHandle_SearchHomingTarget()
{
	if (Movement->bIsHomingProjectile && Movement->HomingTargetComponent.IsValid() == false)
	{
		TArray<FOverlapResult> OverlapResults;
		FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
		bool bResult = GetWorld()->OverlapMultiByChannel(
			OverlapResults,
			GetActorLocation(),
			FQuat::Identity,
			ECCOverlapObjectType,
			FCollisionShape::MakeSphere(SearchHomingTargetRadius),
			CollisionQueryParam);

		if (bResult)
		{
			AActor* ClosestActor = CU::GetClosestActor(OverlapResults, ABaseCharacter::StaticClass(), GetActorLocation(), TNumericLimits<float>::Max());
			if (ClosestActor != nullptr)
			{
				if (USceneComponent* SceneComponent = Cast<USceneComponent>(ClosestActor->GetComponentByClass(USceneComponent::StaticClass())))
				{
					Movement->HomingTargetComponent = SceneComponent;
				}
			}
		}
	}

	if (Movement->HomingTargetComponent.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(SearchHomingTargetTimer);
	}
}

void AProjectileBase::SetGenericTeamId(const FGenericTeamId& NewTeamId)
{
	TeamId = NewTeamId;
	ECCOverlapObjectType = CU::GetECCOverlapByTeamId(NewTeamId);

	TArray<UPrimitiveComponent*> PrimitiveComponents;
	GetComponents<UPrimitiveComponent>(PrimitiveComponents);

	for (auto PrimitiveComponent : PrimitiveComponents)
	{
		PrimitiveComponent->SetCollisionProfileName(CU::GetProjectileCollisionProfileByTeamId(TeamId));
		PrimitiveComponent->SetCollisionObjectType(CU::GetECCCollisionByTeamId(TeamId));
	}
}

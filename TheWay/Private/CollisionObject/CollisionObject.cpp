// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionObject/CollisionObject.h"
#include "Kismet/GameplayStatics.h"
#include "Mode/TheWayGameModeBase.h"
#include "TheWay.h"
#include "Utility/CreataekUtility.h"
#include "Character/BaseCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#if !(UE_BUILD_SHIPPING)
bool ACollisionObject::bDrawDebugInfo = false;
#endif

// Sets default values
ACollisionObject::ACollisionObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACollisionObject::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACollisionObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollisionObject::QueryPointDamageEvent(const float Damage, const FHitResult& HitResult, FPointDamageEvent& OutDamageEvent)
{
	OutDamageEvent.Damage = Damage;
	OutDamageEvent.HitInfo = HitResult;
	OutDamageEvent.ShotDirection = (HitResult.TraceEnd - HitResult.TraceStart).GetSafeNormal();
}

void ACollisionObject::QueryRadialDamageEvent(float BaseDamage, const FVector& Origin, const FRotator& Rotation, float Radius, const TArray<AActor*>& IgnoreActors, ECollisionChannel TraceChannel, TMap<AActor*, FRadialDamageEvent>& OutOverlapInfoMap)
{
	TMap<AActor*, TArray<FHitResult>> OverlapHitResults;

	QueryOverlapHitResults(Origin, Rotation, IgnoreActors, FCollisionShape::MakeSphere(Radius), TraceChannel, OverlapHitResults);

	if (OverlapHitResults.Num() > 0)
	{
		FRadialDamageEvent DmgEvent;

		DmgEvent.DamageTypeClass = UDamageType::StaticClass();
		DmgEvent.Origin = Origin;
		DmgEvent.Params = FRadialDamageParams(BaseDamage, 0.0f, 0.0f, Radius, 0.0f);

		for (TMap<AActor*, TArray<FHitResult>>::TIterator It(OverlapHitResults); It; ++It)
		{
			AActor* const Victim = It.Key();
			TArray<FHitResult> const& ComponentHits = It.Value();

			DmgEvent.ComponentHits = ComponentHits;
			OutOverlapInfoMap.Add(Victim, DmgEvent);
		}
	}
}

void ACollisionObject::QueryOverlapHitResults(const FVector& Origin, const FRotator& Rotation, const TArray<AActor*>& IgnoreActors, const FCollisionShape& CollisionShape, ECollisionChannel TraceChannel, TMap<AActor*, TArray<FHitResult>>& OutOverlapHitResults)
{
	static const FName QueryOverlapHitResultsEventName(TEXT("QueryOverlapHitResults"));
	FCollisionQueryParams QueryParams(QueryOverlapHitResultsEventName, false, this);

	/*QueryParams.AddIgnoredActors(IgnoreActors);
	QueryParams.bIgnoreTouches = true;
	QueryParams.MobilityType = EQueryMobilityType::Dynamic;*/
#if !(UE_BUILD_SHIPPING)
	QueryParams.bDebugQuery = bDrawDebugInfo;
#endif

	UWorld* World = GetWorld();
	check(World != nullptr);

	TArray<FOverlapResult> Overlaps;

	World->OverlapMultiByChannel(Overlaps, Origin, Rotation.Quaternion(), TraceChannel, CollisionShape, QueryParams);

#if !(UE_BUILD_SHIPPING)
	if (bDrawDebugInfo)
	{
		if (CollisionShape.IsSphere())
		{
			UKismetSystemLibrary::DrawDebugSphere(this, Origin, CollisionShape.GetSphereRadius(), 32, FLinearColor::Red, 4, 2);
		}
		else if (CollisionShape.IsCapsule())
		{
			UKismetSystemLibrary::DrawDebugCapsule(this, Origin, CollisionShape.GetCapsuleHalfHeight(), CollisionShape.GetCapsuleRadius(), Rotation, FLinearColor::Red, 4, 2);
		}
		else if (CollisionShape.IsBox())
		{
			UKismetSystemLibrary::DrawDebugBox(this, Origin, CollisionShape.GetExtent(), FLinearColor::Red, Rotation, 4, 2);
		}
	}
#endif

	ATheWayGameModeBase* GameMode = World->GetAuthGameMode< ATheWayGameModeBase>();
	check(GameMode != nullptr);

	FVector TraceOrigin = Origin;
	float MinZCorrection = 10.0f;

	if (CollisionShape.IsSphere())
	{
		TraceOrigin.Z += FMath::Min(CollisionShape.GetSphereRadius() * 0.5f, MinZCorrection);
	}
	else if (CollisionShape.IsCapsule())
	{
		TraceOrigin.Z += FMath::Min(CollisionShape.GetCapsuleHalfHeight() * 0.5f, MinZCorrection);
	}
	else if (CollisionShape.IsBox())
	{
		TraceOrigin.Z += FMath::Min(CollisionShape.GetExtent().Z * 0.5f, MinZCorrection);
	}

	for (int32 Idx = 0; Idx < Overlaps.Num(); ++Idx)
	{
		FOverlapResult const& Overlap = Overlaps[Idx];
		AActor* const OverlapActor = Overlap.GetActor();

		if (OverlapActor && Overlap.Component.IsValid())
		{
			FHitResult Hit;

			if (QueryHitResult(Overlap.Component.Get(), TraceOrigin, IgnoreActors, TraceChannel, Hit))
			{
				TArray<FHitResult>& HitList = OutOverlapHitResults.FindOrAdd(OverlapActor);

				HitList.Add(Hit);
#if !(UE_BUILD_SHIPPING)
				if (bDrawDebugInfo)
				{
					UKismetSystemLibrary::DrawDebugSphere(this, Overlap.Component->Bounds.Origin, 20, 12, FLinearColor::Green, 4, 1);
				}
#endif
			}
#if !(UE_BUILD_SHIPPING)
			else if (bDrawDebugInfo)
			{
				UKismetSystemLibrary::DrawDebugSphere(this, Overlap.Component->Bounds.Origin, 20, 12, FLinearColor::Yellow, 4, 1);
			}
#endif
		}
	}
}

bool ACollisionObject::QueryHitResult(UPrimitiveComponent* TargetComp, FVector const& Origin, const TArray<AActor*>& IgnoreActors, ECollisionChannel TraceChannel, FHitResult& OutHitResult)
{
	UWorld* const World = GetWorld();
	check(World);

	FVector const TraceEnd = TargetComp->Bounds.Origin;
	FVector TraceStart = Origin;

	if (Origin == TraceEnd)
	{
		TraceStart.Z += 0.1f;
	}

	static const FName QueryHitResultEventName(TEXT("QueryHitResult"));
	FCollisionQueryParams QueryParams(QueryHitResultEventName, false, this);

	QueryParams.AddIgnoredActors(IgnoreActors);
	QueryParams.bIgnoreTouches = true;
#if !(UE_BUILD_SHIPPING)
	QueryParams.bDebugQuery = bDrawDebugInfo;
#endif

	TArray<FHitResult> HitResults;

	World->LineTraceMultiByChannel(HitResults, TraceStart, TraceEnd, TraceChannel, QueryParams);

#if !(UE_BUILD_SHIPPING)
	if (bDrawDebugInfo)
	{
		UKismetSystemLibrary::DrawDebugArrow(this, TraceStart, TraceEnd, 10, FLinearColor::Yellow, 4, 1);
	}
#endif

	for (TArray<FHitResult>::TIterator It(HitResults); It; ++It)
	{
		if (It->Component == TargetComp)
		{
			OutHitResult = *It;
			return true;
		}

		if (It->GetActor() == TargetComp->GetOwner())
		{
			continue;
		}

#if !(UE_BUILD_SHIPPING)
		if (bDrawDebugInfo)
		{
			UKismetSystemLibrary::DrawDebugSphere(this, It->ImpactPoint, 20, 12, FLinearColor::Black, 4, 1);
		}
#endif
		OutHitResult = *It;
		UE_LOG(LogTheWay, Log, TEXT("Action to %s (%s) blocked by %s (%s)"),
			*GetNameSafe(TargetComp->GetOwner()),
			*GetNameSafe(TargetComp),
			*GetNameSafe(OutHitResult.GetActor()),
			*GetNameSafe(OutHitResult.Component.Get()));
		return false;
	}

	FVector const FakeHitLoc = TargetComp->GetComponentLocation();
	FVector const FakeHitNorm = (Origin - FakeHitLoc).GetSafeNormal();

	OutHitResult = FHitResult(TargetComp->GetOwner(), TargetComp, FakeHitLoc, FakeHitNorm);

	return true;
}

void ACollisionObject::HandleDamageAction(TArray<FActorDamageEvent>& ActorDamageEvents, AActor* OverrideDealer)
{
	ReturnIfFalse(HasAuthority());

	/*APawn* DamageInstigator = GetInstigator();
	AController* EventInstigator = DamageInstigator != nullptr ? DamageInstigator->GetController() : nullptr;

	for (FActorDamageEvent& ActorDamageEvent : ActorDamageEvents)
	{
		const FDamageEvent* DamageEvent = ActorDamageEvent.DamageEvent;
		const FHitResult* HitResult = nullptr;
		bool bRadialDamage = false;

		if (DamageEvent->GetTypeID() == FPointDamageEvent::ClassID)
		{
			HitResult = &Cast<FPointDamageEvent>(DamageEvent)->HitInfo;
		}
		else if (DamageEvent->GetTypeID() == FRadialDamageEvent::ClassID)
		{
			bRadialDamage = true;
			const FRadialDamageEvent* DmgEvt = Cast<FRadialDamageEvent>(DamageEvent);
			if (DmgEvt->ComponentHits.Num() > 0)
			{
				HitResult = &DmgEvt->ComponentHits[0];
			}
		}

		if (HitResult == nullptr)
		{
			continue;
		}

		AActor* Victim = ActorDamageEvent.Victim;
		ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Victim);

		if (BaseCharacter != nullptr)
		{
			//UGameplayStatics::ApplyDamage(BaseCharacter, DamageEvent)
		}
	}*/
}

APeriodicDamageSphereCollision::APeriodicDamageSphereCollision()
{
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));

	ElapseTime = 0.0f;
}

void APeriodicDamageSphereCollision::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	IGenericTeamAgentInterface* TeamAgentInterface = Cast<IGenericTeamAgentInterface>(GetOwner());
	FGenericTeamId OwnerTeamId = TeamAgentInterface != nullptr ? TeamAgentInterface->GetGenericTeamId() : FGenericTeamId();
	Collision->SetCollisionProfileName(CU::GetOverlapCollisionProfileByTeamId(OwnerTeamId));
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	GetWorldTimerManager().SetTimer(TimerHandle_NextDamage, this, &APeriodicDamageSphereCollision::ApplyDamage, PeriodTime, true);
}

void APeriodicDamageSphereCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapseTime += DeltaTime;

	if (ElapseTime >= DurationTime)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_NextDamage);
		Destroy();
	}
}

void APeriodicDamageSphereCollision::ApplyDamage()
{
	const TArray<FOverlapInfo>& OverlapInfos = Collision->GetOverlapInfos();

	for (const FOverlapInfo& OverlapInfo : OverlapInfos)
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(OverlapInfo.OverlapInfo.Actor))
		{
			FImpulseDamageEvent DamageEvent;
			DamageEvent.Damage = GetFinalAttackDamage();
			DamageEvent.HitInfo = OverlapInfo.OverlapInfo;
			DamageEvent.ShotDirection = (OverlapInfo.OverlapInfo.Actor->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();

			Character->TakeDamage(GetFinalAttackDamage(), DamageEvent, (OwnerCharacter ? OwnerCharacter->Controller : nullptr), this);
			Character->GetCharacterMovement()->AddImpulse(DamageEvent.ShotDirection * KnockbackPower, true);
		}
	}
}

float APeriodicDamageSphereCollision::GetFinalAttackDamage()
{
	float FinalDamage = (OwnerCharacter ? OwnerCharacter->GetFinalAttckDamage() : Damage);
	return FinalDamage;
}

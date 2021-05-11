// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Spawner/ProjectileSpawnComponent.h"
#include "Projectile/ProjectileBase.h"
#include "Utility/CreataekUtility.h"

// Sets default values for this component's properties
UProjectileSpawnComponent::UProjectileSpawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	ShootAngleOffset = FVector::ZeroVector;
	ProjectileInitialSpeed = 0.0f;
}

// Called when the game starts
void UProjectileSpawnComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UProjectileSpawnComponent::Spawn(UClass* Class, FVector Location, FVector ShootDirection, APawn* Instigator)
{
	ReturnIfTrue(OwnerLevel < RequiredOwnerMinLevel || OwnerLevel > RequiredOwnerMaxLevel);

	if (ShootDelay == 0.0f)
	{
		SpawnInternal(Class, Location, ShootDirection, Instigator);
	}
	else
	{
		FTimerHandle Timer;
		FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &UProjectileSpawnComponent::SpawnInternal, Class, Location, ShootDirection, Instigator);

		GetWorld()->GetTimerManager().SetTimer(Timer, RespawnDelegate, ShootDelay, false);
	}
}

void UProjectileSpawnComponent::SpawnInternal(UClass* Class, FVector Location, FVector ShootDirection, APawn* Instigator)
{
	FQuat FinalQuaternion = Instigator->GetActorQuat() * FQuat::MakeFromEuler(ShootAngleOffset);
	FVector FinalShootDirection = FinalQuaternion.GetForwardVector();
	FQuat BetweenQuat = FQuat::FindBetweenVectors(FVector::ForwardVector, Instigator->GetActorForwardVector());
	FVector BetweenAxis = BetweenQuat.GetRotationAxis();
	float BetweenAngle = FMath::RadiansToDegrees(BetweenQuat.GetAngle());
	FVector FinalLocationOffset = LocationOffset.RotateAngleAxis(BetweenAngle, BetweenAxis);
	FVector FinalLocation = Location + FinalLocationOffset;
	FActorSpawnParameters SpawnParams;

	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = Instigator;

	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>((OverrideProjectileClass.Get() == nullptr ? Class : OverrideProjectileClass), FinalLocation, FRotator(0.0f), SpawnParams);

	Projectile->Shoot(FinalShootDirection, ProjectileInitialSpeed);
}
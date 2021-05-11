// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapon/RangedWeapon/RangedWeaponBase.h"
#include "Character/BaseCharacter.h"
#include "Projectile/ProjectileBase.h"
#include "Components/Spawner/ProjectileSpawnComponent.h"

ARangedWeaponBase::ARangedWeaponBase()
{
}

void ARangedWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void ARangedWeaponBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GetComponents<UProjectileSpawnComponent>(ProjectileSpawnComponents);

	for (auto& ProjectileSpawner : ProjectileSpawnComponents)
	{
		ProjectileSpawner->SetOwnerLevel(Level);
	}
}

void ARangedWeaponBase::StartAttack()
{
}

void ARangedWeaponBase::DecideAttack()
{
	FVector ShootDirection = OwnerCharacter->GetActorForwardVector();

	ShootDirection.Z = 0.0f;
	ShootDirection.Normalize();

	for (auto& ProjectileSpawner : ProjectileSpawnComponents)
	{
		ProjectileSpawner->Spawn(ProjectileClass, GetActorLocation(), ShootDirection, OwnerCharacter);
	}
}

void ARangedWeaponBase::LevelUp()
{
	Super::LevelUp();

	for (auto& ProjectileSpawner : ProjectileSpawnComponents)
	{
		ProjectileSpawner->SetOwnerLevel(Level);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "TheWayAnimNotify.generated.h"

/**
 *
 */
UCLASS(meta = (DisplayName = "Weapon Attack Notify"))
class UTheWayAnimNotify_WeaponAttack : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation) override;
};

USTRUCT()
struct FProjectileSet_AnimNotify_SpawnProjectile
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectileBase> ProjectileClass;

	UPROPERTY(EditAnywhere)
	FVector LocationOffset;

	UPROPERTY(EditDefaultsOnly)
	FVector ShootAngleOffset; // Degree ¥‹¿ß

	UPROPERTY(EditAnywhere)
	float ProjectileInitialSpeed;

	UPROPERTY(EditAnywhere)
	FName LocationSocketName;
};

UCLASS(meta = (DisplayName = "Spawn Projectile Notify"))
class UTheWayAnimNotify_SpawnProjectile : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation) override;

protected:
	UPROPERTY(EditAnywhere)
	TArray<FProjectileSet_AnimNotify_SpawnProjectile> ProjectileArray;
};

UCLASS(meta = (DisplayName = "Spawn Collision Object Notify"))
class UTheWayAnimNotify_SpawnCollisionObject : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation) override;

protected:
	UPROPERTY(EditAnywhere, meta = (MetaClass = "CollisionObject", ExactClass = false))
	FSoftClassPath CollisionObjectClass;

	UPROPERTY(EditAnywhere)
	FVector SpawnLocationOffset;

	UPROPERTY(EditAnywhere)
	FRotator SpawnRotationOffset;
};
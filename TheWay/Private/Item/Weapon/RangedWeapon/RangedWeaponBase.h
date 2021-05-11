// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Weapon/WeaponBase.h"
#include "RangedWeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class ARangedWeaponBase : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	ARangedWeaponBase();

public:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	virtual void StartAttack() override;
	virtual void DecideAttack() override;

	virtual void LevelUp() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AProjectileBase> ProjectileClass;

	UPROPERTY()
	TArray<class UProjectileSpawnComponent*> ProjectileSpawnComponents;

};

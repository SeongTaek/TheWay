// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Weapon/WeaponBase.h"
#include "GenericTeamAgentInterface.h"
#include "MeleeWeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class AMeleeWeaponBase : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AMeleeWeaponBase();

public:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	virtual void StartAttack() override;
	virtual void DecideAttack() override;

	virtual void OnEquip(class ABaseCharacter* InOwnerCharacter) override;

protected:
	void AttackCheckCollision();

protected:
	UPROPERTY(EditAnywhere)
	int AttackCheckCount;

	UPROPERTY(EditAnywhere)
	float AttackCheckRate;

	UPROPERTY(EditAnywhere)
	float AttackCheckFirstDelay;

	TArray<UPrimitiveComponent*> AttackCheckCollisions;
	FTimerHandle AttackCheckTimer;
	int CurrentAttackCheckCount;

	/*
	* IGenericTeamAgentInterface
	*/
public:
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamId) override;
};

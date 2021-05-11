// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "Components/WeaponStatComponent.h"
#include "GenericTeamAgentInterface.h"
#include "WeaponBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnWeaponLevelUp);

/**
 * 
 */
UCLASS()
class AWeaponBase : public AItemBase, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:
	static AWeaponBase* Spawn(AActor* Owner, TSharedPtr<const struct FWeaponData> WeaponData);

public:
	AWeaponBase();

public:
	virtual void BeginPlay() override;

	virtual void StartAttack();
	virtual void DecideAttack();

	virtual void OnEquip(class ABaseCharacter* InOwnerCharacter);
	virtual void Unequip();

	virtual void LevelUp();
	bool IsMaxLevel();

	class UWeaponStatComponent* GetWeaponStatComponent() const { return WeaponStatComponent; }
	float GetFinalAttackDamage() const;
	int32 GetLevel() const { return Level ; }

	void AttachToOwnerCharacter();

	virtual void SetItemID(int32 Id, bool bUpdateProperties = true) override;

protected:
	virtual void CacheItemCsv();

public:
	FOnWeaponLevelUp OnWeaponLevelUp;

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UWeaponStatComponent* WeaponStatComponent;

	UPROPERTY(EditAnywhere, Category = Weapon)
	FName AttachedSocketName;

	FTimerHandle AutomaticAttackTimer;

	int32 WeaponId;
	int32 Level = 1;

	TWeakPtr<const struct FWeaponData> WeaponDataCsv;

	/*
	* IGenericTeamAgentInterface
	*/
public:
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamId) override { TeamId = NewTeamId; }
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }

protected:
	FGenericTeamId TeamId;
};

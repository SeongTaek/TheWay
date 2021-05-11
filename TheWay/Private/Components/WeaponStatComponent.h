// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UWeaponStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	bool GetAutomaticAttack() const { return bAutomaticAttack; }
	float GetAutomaticAttackSpeed() const { return AutomaticAttackSpeed; }
	float GetAttackDamage() const { return AttackDamage; }
	float GetAttackSpeed() const { return AttackSpeed; }
	float GetAttackRange() const { return AttackRange; }

	void SetStatsData(const TSharedPtr<const struct FWeaponStatsData>);

private:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	bool bAutomaticAttack;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AutomaticAttackSpeed; // 1.0f = 1초에 한번 공격.

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AttackDamage;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AttackSpeed;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AttackRange;
};

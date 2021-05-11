// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHPChanged, float, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:	
	float GetAttackDamage() const { return AttackDamage; }
	float GetAttackSpeed() const { return AttackSpeed; }
	float GetCurrentHp() const { return CurrentHp; }
	float GetMaxHp() const { return MaxHp; }
	float GetPatrolRadius() const { return PatrolRadius; }
	float GetDetectRadius() const { return DetectRadius; }

	/// <summary>
	/// 데이터 테이블 Id를 설정합니다.
	/// </summary>
	/// <param name="Id"></param>
	/// <param name="bUpdateProperties">참이면 설정된 테이블 Id 값으로 데이터를 갱신합니다.</param>
	void SetStatsID(int32 Id, bool bUpdateProperties = true);
	void AddHp(float Value);
	void TakeDamage(float Damage);

public:
	FOnHPChanged OnHPChanged;
	FOnHPIsZeroDelegate OnHPIsZero;

private:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHp;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float MaxHp;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AttackDamage;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AttackSpeed;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float PatrolRadius;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float DetectRadius;

	int32 StatsId;

	TSharedPtr<const struct FCharacterStatsData> CharacterStatsDataCsv;
};

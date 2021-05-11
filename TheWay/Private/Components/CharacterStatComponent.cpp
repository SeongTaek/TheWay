// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterStatComponent.h"
#include "DataTable/DataTableManager.h"
#include "DataTable/TheWayDataTable.h"
#include "Utility/CreataekUtility.h"

// Sets default values for this component's properties
UCharacterStatComponent::UCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}


// Called when the game starts
void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
}


void UCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UCharacterStatComponent::SetStatsID(int32 Id, bool bUpdateProperties)
{
	if (StatsId != Id)
	{
		ReturnIfNull(UDataTableManager::Get(this));
		CharacterStatsDataCsv = UDataTableManager::Get(this)->GetTableCsv<FCharacterStatsData>(Id);
	}
	ReturnIfNull(CharacterStatsDataCsv);
	StatsId = Id;

	if (bUpdateProperties)
	{
		CurrentHp = CharacterStatsDataCsv->Hp;
		MaxHp = CharacterStatsDataCsv->MaxHp;
		AttackDamage = CharacterStatsDataCsv->AttackDamage;
		AttackSpeed = CharacterStatsDataCsv->AttackSpeed;
		PatrolRadius = CharacterStatsDataCsv->PatrolRadius;
		DetectRadius = CharacterStatsDataCsv->DetectRadius;
	}
}

void UCharacterStatComponent::AddHp(float Value)
{
	CurrentHp += Value;
	if (CurrentHp > MaxHp)
	{
		CurrentHp = MaxHp;
	}
	OnHPChanged.Broadcast(CurrentHp, MaxHp);
}

void UCharacterStatComponent::TakeDamage(float Damage)
{
	CurrentHp = FMath::Clamp<float>(CurrentHp - Damage, 0.0f, MaxHp);

	OnHPChanged.Broadcast(CurrentHp, MaxHp);
	if (CurrentHp <= 0.0f)
	{
		OnHPIsZero.Broadcast();
	}
}


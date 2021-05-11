// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WeaponStatComponent.h"
#include "Item/Weapon/WeaponBase.h"
#include "DataTable/DataTableManager.h"
#include "DataTable/TheWayDataTable.h"

// Sets default values for this component's properties
UWeaponStatComponent::UWeaponStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	bAutomaticAttack = false;
	AutomaticAttackSpeed = 1.0;
	AttackDamage = 10.0f;
	AttackSpeed = 1.0f;
	AttackRange = 2000.0f;
}


// Called when the game starts
void UWeaponStatComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...

}

void UWeaponStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UWeaponStatComponent::SetStatsData(const TSharedPtr<const struct FWeaponStatsData> WeaponStatsData)
{
	bAutomaticAttack = WeaponStatsData->bAutomaticAttack;
	AutomaticAttackSpeed = WeaponStatsData->AutomaticAttackSpeed;
	AttackDamage = WeaponStatsData->AttackDamage;
	AttackSpeed = WeaponStatsData->AttackSpeed;
	AttackRange = WeaponStatsData->AttackRange;
}


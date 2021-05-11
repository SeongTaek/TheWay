// Fill out your copyright notice in the Description page of Project Settings.


#include "DataTable/DataTableManager.h"
#include "Framework/TheWayGameInstance.h"

UDataTableManager::UDataTableManager()
{
}

void UDataTableManager::Init()
{
	FString DataPath = TEXT("/Game/DataTable/item.item");
	UDataTable* DataTable_Item = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, *DataPath));
	TArray<FItemData*> Item_OutRowArray;
	DataTable_Item->GetAllRows(TEXT("DataTable_Item"), Item_OutRowArray);
	AddDataTable(Item_OutRowArray);

	DataPath = TEXT("/Game/DataTable/item_weapon.item_weapon");
	UDataTable* DataTable_Item_Weapon = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, *DataPath));
	TArray<FWeaponData*> Item_Weapon_OutRowArray;
	DataTable_Item_Weapon->GetAllRows(TEXT("DataTable_Item_Weapon"), Item_Weapon_OutRowArray);
	AddDataTable(Item_Weapon_OutRowArray);

	DataPath = TEXT("/Game/DataTable/item_weapon_stats.item_weapon_stats");
	UDataTable* DataTable_Item_Weapon_Stats = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, *DataPath));
	TArray<FWeaponStatsData*> Item_Weapon_Stats_OutRowArray;
	DataTable_Item_Weapon_Stats->GetAllRows(TEXT("DataTable_Item_Weapon_Stats"), Item_Weapon_Stats_OutRowArray);
	AddDataTable(Item_Weapon_Stats_OutRowArray);

	DataPath = TEXT("/Game/DataTable/character.character");
	UDataTable* DataTable_Character = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, *DataPath));
	TArray<FCharacterData*> Character_OutRowArray;
	DataTable_Character->GetAllRows(TEXT("DataTable_Character"), Character_OutRowArray);
	AddDataTable(Character_OutRowArray);

	DataPath = TEXT("/Game/DataTable/character_stats.character_stats");
	UDataTable* DataTable_Character_Stats = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, *DataPath));
	TArray<FCharacterStatsData*> Character_Stats_OutRowArray;
	DataTable_Character_Stats->GetAllRows(TEXT("DataTable_Character_Stats"), Character_Stats_OutRowArray);
	AddDataTable(Character_Stats_OutRowArray);

	DataPath = TEXT("/Game/DataTable/spawner.spawner");
	UDataTable* DataTable_Spawner = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, *DataPath));
	TArray<FSpawnerData*> Spawner_OutRowArray;
	DataTable_Spawner->GetAllRows(TEXT("DataTable_Spawner"), Spawner_OutRowArray);
	AddDataTable(Spawner_OutRowArray);
}

UDataTableManager* UDataTableManager::Get(const UObject* InWorldContextObj)
{
	if (InWorldContextObj == nullptr)
	{
		return nullptr;
	}

	UWorld* World = InWorldContextObj->GetWorld();
	UTheWayGameInstance* TheWayGameInstance = World ? World->GetGameInstance<UTheWayGameInstance>() : nullptr;

	return TheWayGameInstance ? TheWayGameInstance->DataTableManager : nullptr;
}

UDataTableManager* UDataTableManager::Get(const UTheWayGameInstance* InGameInstance)
{
	if (InGameInstance == nullptr)
	{
		return nullptr;
	}

	return InGameInstance->DataTableManager;
}

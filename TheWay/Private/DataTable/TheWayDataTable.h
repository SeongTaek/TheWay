// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TheWayDataTable.generated.h"

USTRUCT(BlueprintType)
struct FProbabilityData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 Id;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 Probability;
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 Id;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	FString Name;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	FString Blueprint_File;
};

USTRUCT(BlueprintType)
struct FWeaponData : public FItemData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Stats_Id;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 MaxLevel;
};

USTRUCT(BlueprintType)
struct FWeaponStatsData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 Id;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 Level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	bool bAutomaticAttack;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float AutomaticAttackSpeed;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float AttackDamage;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float AttackSpeed;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float AttackRange;
};

USTRUCT(BlueprintType)
struct FDropItemData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 Id;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 Probability;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 MinDrop;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 MaxDrop;
};

USTRUCT(BlueprintType)
struct FCharacterData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 Id;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	FString Name;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	FString Blueprint_File;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Stats_Id;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 EquippedWeapon_Id;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	TArray<FDropItemData> DropItemList;
};

USTRUCT(BlueprintType)
struct FCharacterStatsData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 Id;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 Hp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 MaxHp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float AttackDamage;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float AttackSpeed;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float AttackRange;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 PatrolRadius;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 DetectRadius;
};

USTRUCT(BlueprintType)
struct FSpawnerData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 Id;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	FString Name;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 Level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	FString Blueprint_File;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	TArray<FProbabilityData> SpawnProbabilityList;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float Interval;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 AreaRadius;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 MaxNum;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 SoulNumToNextLevel;
};

USTRUCT(BlueprintType)
struct FInGameWaveData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 Wave;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float Time;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 SpawnerLevel;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 BossCharacterId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	TArray<int32> FriendlyCharacterIdList;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	TArray<int32> HostileCharacterIdList;
};
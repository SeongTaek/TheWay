// Fill out your copyright notice in the Description page of Project Settings.


#include "TheWayGameInstance.h"
#include "DataTable/DataTableManager.h"

UTheWayGameInstance::UTheWayGameInstance()
{
	DataTableManager = CreateDefaultSubobject<UDataTableManager>(TEXT("DataTableManager"));
}

void UTheWayGameInstance::Init()
{
	Super::Init();

	UDataTableManager::Get(this)->Init();
}

void UTheWayGameInstance::StartGameInstance()
{
	Super::StartGameInstance();
}

TStatId UTheWayGameInstance::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTheWayGameInstance, STATGROUP_Tickables);
}

void UTheWayGameInstance::Tick(float DeltaSeconds)
{
}
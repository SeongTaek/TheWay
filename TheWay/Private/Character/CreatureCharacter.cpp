// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CreatureCharacter.h"
#include "Controller/BaseAIController.h"
#include "Utility/CreataekUtility.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Item/Weapon/WeaponBase.h"
#include "DataTable/DataTableManager.h"
#include "DataTable/TheWayDataTable.h"
#include "TheWay.h"

// Sets default values
ACreatureCharacter::ACreatureCharacter()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
}

void ACreatureCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ACreatureCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACreatureCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetGenericTeamId(FGenericTeamId(ETeamAttitude::Hostile));
}

void ACreatureCharacter::CacheCharacterCsv()
{
	ReturnIfNull(UDataTableManager::Get(this));
	CharacterDataCsv = UDataTableManager::Get(this)->GetTableCsv<FCharacterData>(CharacterId);
}

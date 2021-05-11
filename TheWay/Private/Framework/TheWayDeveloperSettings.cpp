// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/TheWayDeveloperSettings.h"

UTheWayDeveloperSettings::UTheWayDeveloperSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CharacterClassBasePath = TEXT("/Game/Characters/");
	WeaponClassBasePath = TEXT("/Game/Blueprint/Weapon/");
	ItemClassBasePath = TEXT("/Game/Blueprint/Item/");
	SpawnerClassBasePath = TEXT("/Game/Blueprint/Spawner/");

	MouseSensitivity = 1.0f;
}
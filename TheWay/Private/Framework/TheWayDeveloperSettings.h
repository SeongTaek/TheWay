// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "TheWayDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "Project The Way"))
class UTheWayDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UTheWayDeveloperSettings(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(config, EditAnywhere, meta = (AllowedClasses = "World"))
	FSoftObjectPath InGameLevel;

	UPROPERTY(config, EditAnywhere, meta = (AllowedClasses = "World"))
	FSoftObjectPath MainMenuLevel;

	UPROPERTY(config, EditAnywhere)
	FString CharacterClassBasePath;

	UPROPERTY(config, EditAnywhere)
	FString WeaponClassBasePath;

	UPROPERTY(config, EditAnywhere)
	FString ItemClassBasePath;

	UPROPERTY(config, EditAnywhere)
	FString SpawnerClassBasePath;

	UPROPERTY(config, EditAnywhere)
	float MouseSensitivity;
};

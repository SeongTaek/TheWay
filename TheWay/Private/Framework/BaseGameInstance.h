// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BaseGameInstance.generated.h"

enum class EShutdownPriority
{
	Never,
	OkIfInevitable,
	OK,
	Must,
	Max,
};

/**
 *
 */
UCLASS()
class UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	virtual void Init() override;

	virtual void RestartGameInstance() {}

	virtual void OnPostLoadMap() {}
	virtual void OnWorldBeginTearingDown() {}
	virtual void LoadIntro();
	virtual void LoadLobby();
	virtual void LoadMap(const FString& LoadMap, const FString& Mode);

public:
	virtual EShutdownPriority GetShutdownPriority() const { return EShutdownPriority::OkIfInevitable; }

};

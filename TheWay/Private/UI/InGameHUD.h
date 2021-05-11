// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseHUD.h"
#include "InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class AInGameHUD : public ABaseHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	virtual void PostInitializeComponents() override;

	class UInGameHUDWidget* GetInGameHUDWidget() const { return InGameHUDWidget; }

protected:
	virtual void LoadWidget() override;

protected:
	class UInGameHUDWidget* InGameHUDWidget;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseHUD.h"
#include "MainMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class AMainMenuHUD : public ABaseHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	virtual void LoadWidget();

protected:
	class UMainMenuHUDWidget* MainMenuHUDWidget;
};

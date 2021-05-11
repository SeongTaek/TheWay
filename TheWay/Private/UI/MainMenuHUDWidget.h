// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUserWidget.h"
#include "MainMenuHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class UMainMenuHUDWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;

protected:
	UFUNCTION()
	void OnStartButtonClicked();

	UFUNCTION()
	void OnRankingButtonClicked();

	UFUNCTION()
	void OnQuitButtonClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Start;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Ranking;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Quit;
};

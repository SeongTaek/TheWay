// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUserWidget.h"
#include "InGameFinishPopup.generated.h"

/**
 * 
 */
UCLASS()
class UInGameFinishPopup : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
protected:
	UFUNCTION()
	void OnMainMenuButtonClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Wave;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_TotalSouls;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_MainMenu;
};

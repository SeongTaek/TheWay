// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUserWidget.h"
#include "GamePausePopup.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPopupClosed);

/**
 * 
 */
UCLASS()
class UGamePausePopup : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	UGamePausePopup();

	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
	void OnResumeButtonClicked();

	UFUNCTION()
	void OnMenuButtonClicked();

	UFUNCTION()
	void OnQuitButtonClicked();

public:
	FOnPopupClosed OnPopupClosed;

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Resume;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Menu;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Quit;
};

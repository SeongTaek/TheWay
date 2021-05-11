// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUserWidget.h"
#include "InGameHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class UInGameHUDWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void BindPlayerState(class ABasePlayerState* InPlayerState);

protected:
	void UpdatePlayerState();

	/*
	* UI Widget
	*/
protected:
	UPROPERTY(meta = (BindWidget))
	class USoulPointsLabelWidget* SoulPointsLabel;

protected:
	TWeakObjectPtr<class ABasePlayerState> PlayerState;
};

UCLASS()
class UHealthBarWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
protected:
	UFUNCTION()
	void OnCharacterChanged(class ABaseCharacter* OldCharacter, class ABaseCharacter* NewCharacter);

	UFUNCTION()
	void OnHPChanged(float Current, float Max);

protected:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_Health;

	FDelegateHandle OnViewCharacterChangedHandle;
};
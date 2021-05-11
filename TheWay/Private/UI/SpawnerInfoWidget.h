// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUserWidget.h"
#include "SpawnerInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class USpawnerInfoWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UFUNCTION()
	void OnLevelUpClicked();

	void UpdateWidget();
	void OnPickActor(TWeakObjectPtr<AActor> PickedActor);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Name;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Level;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Interval;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_SpawnProbabilityList;
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_LevelUp;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_LevelUpCost;

	TWeakObjectPtr<class ACharacterSpawner> TargetSpawner;
};

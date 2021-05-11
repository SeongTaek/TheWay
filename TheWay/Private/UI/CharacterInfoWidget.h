// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUserWidget.h"
#include "CharacterInfoWidget.generated.h"

/**
 *
 */
UCLASS()
class UCharacterInfoWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	void UpdateWidget();
	void OnPickActor(TWeakObjectPtr<AActor> PickedActor);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Name;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_HP;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_AD;

	TWeakObjectPtr<class ABaseCharacter> TargetCharacter;
};

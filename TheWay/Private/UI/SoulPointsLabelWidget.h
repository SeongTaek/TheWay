// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUserWidget.h"
#include "SoulPointsLabelWidget.generated.h"

/**
 * 
 */
UCLASS()
class USoulPointsLabelWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetSoulPoints(int32 SoulPoints);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_SoulsPoint;
};

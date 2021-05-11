// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUserWidget.h"
#include "NoticeWaveWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNoticeWaveWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	void SetWave(int32 Wave);
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Wave;
};

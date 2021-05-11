// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "UIClassSetting.generated.h"

/**
 * 
 */
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "UI Class Setting"))
class UUIClassSetting : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(config, Category = "HUD", EditAnywhere, meta = (MetaClass = "InGameHUDWidget", ExactClass = false))
	FSoftClassPath InGameHUDWidgetClass;

	UPROPERTY(config, Category = "HUD", EditAnywhere, meta = (MetaClass = "MainMenuHUDWidget", ExactClass = false))
	FSoftClassPath MainMenuHUDWidgetClass;

	UPROPERTY(config, Category = "Widget", EditAnywhere, meta = (MetaClass = "NoticeWaveWidget", ExactClass = false))
	FSoftClassPath NoticeWaveWidgetClass;

	UPROPERTY(config, Category = "Popup", EditAnywhere, meta = (MetaClass = "InGameFinishPopup", ExactClass = false))
	FSoftClassPath InGameFinishPopupClass;

	UPROPERTY(config, Category = "Popup", EditAnywhere, meta = (MetaClass = "GamePausePopup", ExactClass = false))
	FSoftClassPath GamePausePopupClass;
};

#define UIClass GetDefault<UUIClassSetting>()
#define LoadClass(Path) (Path).TryLoadClass<UUserWidget>()
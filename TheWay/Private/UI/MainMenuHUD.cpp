// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuHUD.h"
#include "UI/InGameHUDWidget.h"
#include "UI/UIClassSetting.h"
#include "UI/MainMenuHUDWidget.h"
#include "TheWay.h"

void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AMainMenuHUD::LoadWidget()
{
	Super::LoadWidget();

	const UUIClassSetting* ClassSetting = UIClass;
	UClass* MainMenuHUDClass = LoadClass(ClassSetting->MainMenuHUDWidgetClass);
	if (MainMenuHUDClass)
	{
		MainMenuHUDWidget = CreateWidget<UMainMenuHUDWidget>(GetWorld(), MainMenuHUDClass);
		MainMenuHUDWidget->AddToViewport((int32)EWidgetOrder::DefaultHUD);
	}
}

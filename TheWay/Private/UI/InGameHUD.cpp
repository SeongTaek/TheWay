// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameHUD.h"
#include "UI/InGameHUDWidget.h"
#include "UI/UIClassSetting.h"
#include "TheWay.h"
#include "Framework/BasePlayerState.h"

void AInGameHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AInGameHUD::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AInGameHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AInGameHUD::LoadWidget()
{
	Super::LoadWidget();

	const UUIClassSetting* ClassSetting = UIClass;
	UClass* InGameHUDClass = LoadClass(ClassSetting->InGameHUDWidgetClass);
	if (InGameHUDClass)
	{
		ABasePlayerState* BasePlayerState = Cast<ABasePlayerState>(GetOwningPlayerController()->PlayerState);

		InGameHUDWidget = CreateWidget<UInGameHUDWidget>(GetWorld(), InGameHUDClass);
		InGameHUDWidget->BindPlayerState(BasePlayerState);
		BasePlayerState->OnPlayerStateChanged.Broadcast();
		InGameHUDWidget->AddToViewport((int32)EWidgetOrder::InGameHUD);
	}
}

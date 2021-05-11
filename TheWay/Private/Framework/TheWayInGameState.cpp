// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/TheWayInGameState.h"
#include "TheWay.h"
#include "UI/UIClassSetting.h"
#include "UI/NoticeWaveWidget.h"
#include "UI/InGameFinishPopup.h"
#include "UI/GamePausePopup.h"

ATheWayInGameState::ATheWayInGameState()
{
}

void ATheWayInGameState::NoticeWaveMessage(const int32 Wave)
{
	CurrentWave = Wave;

	UClass* NoticeWaveWidgetClass = LoadClass(UIClass->NoticeWaveWidgetClass);
	if (NoticeWaveWidgetClass)
	{
		UNoticeWaveWidget* NoticeWaveWidget = CreateWidget<UNoticeWaveWidget>(GetWorld(), NoticeWaveWidgetClass);
		NoticeWaveWidget->SetWave(Wave);
		NoticeWaveWidget->AddToViewport((int32)EWidgetOrder::NoticeMessage);
	}
}

UInGameFinishPopup* ATheWayInGameState::OpenGameFinishPopup()
{
	UClass* FinishPopupClass = LoadClass(UIClass->InGameFinishPopupClass);
	if (FinishPopupClass)
	{
		UInGameFinishPopup* FinishPopup = CreateWidget<UInGameFinishPopup>(GetWorld(), FinishPopupClass);
		FinishPopup->AddToViewport((int32)EWidgetOrder::NoticeMessage);

		return FinishPopup;
	}

	return nullptr;
}

UGamePausePopup* ATheWayInGameState::OpenGamePausePopup()
{
	UClass* PausePopupClass = LoadClass(UIClass->GamePausePopupClass);
	if (PausePopupClass)
	{
		UGamePausePopup* PausePopup = CreateWidget<UGamePausePopup>(GetWorld(), PausePopupClass);
		PausePopup->AddToViewport((int32)EWidgetOrder::NoticeMessage);

		return PausePopup;
	}

	return nullptr;
}

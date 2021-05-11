// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuHUDWidget.h"
#include "Framework/TheWayDeveloperSettings.h"
#include "UI/UIClassSetting.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Controller/BasePlayerController.h"

void UMainMenuHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GetPlayerController()->bShowMouseCursor = true;

	Btn_Start->OnClicked.AddDynamic(this, &UMainMenuHUDWidget::OnStartButtonClicked);
	Btn_Ranking->OnClicked.AddDynamic(this, &UMainMenuHUDWidget::OnRankingButtonClicked);
	Btn_Quit->OnClicked.AddDynamic(this, &UMainMenuHUDWidget::OnQuitButtonClicked);
}

void UMainMenuHUDWidget::OnStartButtonClicked()
{
	UGameplayStatics::OpenLevel(this, FName(*GetDefault<UTheWayDeveloperSettings>()->InGameLevel.GetLongPackageName()), TRAVEL_Absolute);
}

void UMainMenuHUDWidget::OnRankingButtonClicked()
{

}

void UMainMenuHUDWidget::OnQuitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

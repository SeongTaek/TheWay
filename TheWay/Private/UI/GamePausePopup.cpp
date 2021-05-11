// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GamePausePopup.h"
#include "Controller/BasePlayerController.h"
#include "Framework/TheWayDeveloperSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

UGamePausePopup::UGamePausePopup()
{
}

void UGamePausePopup::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Btn_Resume->OnClicked.AddDynamic(this, &UGamePausePopup::OnResumeButtonClicked);
	Btn_Menu->OnClicked.AddDynamic(this, &UGamePausePopup::OnMenuButtonClicked);
	Btn_Quit->OnClicked.AddDynamic(this, &UGamePausePopup::OnQuitButtonClicked);

	GetPlayerController()->SetPause(true);
	GetPlayerController()->bShowMouseCursor = true;
}

void UGamePausePopup::NativeDestruct()
{
	OnPopupClosed.Broadcast();
}

void UGamePausePopup::OnResumeButtonClicked()
{
	GetPlayerController()->SetPause(false);
	GetPlayerController()->bShowMouseCursor = false;
	RemoveFromViewport();
}

void UGamePausePopup::OnMenuButtonClicked()
{
	UGameplayStatics::OpenLevel(this, FName(*GetDefault<UTheWayDeveloperSettings>()->MainMenuLevel.GetLongPackageName()), TRAVEL_Absolute);
}

void UGamePausePopup::OnQuitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

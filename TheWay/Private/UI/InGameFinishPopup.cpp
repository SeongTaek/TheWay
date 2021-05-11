// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameFinishPopup.h"
#include "Framework/TheWayDeveloperSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/TheWayInGameState.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Controller/BasePlayerController.h"
#include "Framework/BasePlayerState.h"

void UInGameFinishPopup::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Btn_MainMenu->OnClicked.AddDynamic(this, &UInGameFinishPopup::OnMainMenuButtonClicked);

	Txt_Wave->SetText(FText::AsNumber(GetWorld()->GetGameState<ATheWayInGameState>()->GetCurrentWave()));
	Txt_TotalSouls->SetText(FText::AsNumber(GetPlayerController()->GetLocalPlayerState()->GetTotalSoulAbsorptionCount()));

	GetPlayerController()->bShowMouseCursor = true;
}

void UInGameFinishPopup::OnMainMenuButtonClicked()
{
	UGameplayStatics::OpenLevel(this, FName(*GetDefault<UTheWayDeveloperSettings>()->MainMenuLevel.GetLongPackageName()), TRAVEL_Absolute);
}

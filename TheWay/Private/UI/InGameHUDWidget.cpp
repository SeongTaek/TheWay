// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameHUDWidget.h"
#include "Utility/CreataekUtility.h"
#include "Framework/BasePlayerState.h"
#include "Components/TextBlock.h"
#include "UI/SoulPointsLabelWidget.h"
#include "Character/BaseCharacter.h"
#include "Controller/BasePlayerController.h"
#include "Components/CharacterStatComponent.h"
#include "Components/ProgressBar.h"

void UInGameHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UInGameHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInGameHUDWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UInGameHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInGameHUDWidget::BindPlayerState(ABasePlayerState* InPlayerState)
{
	PlayerState = InPlayerState;
	PlayerState->OnPlayerStateChanged.AddUObject(this, &UInGameHUDWidget::UpdatePlayerState);
}

void UInGameHUDWidget::UpdatePlayerState()
{
	ReturnIfNull(PlayerState);

	SoulPointsLabel->SetSoulPoints(PlayerState->GetSoulAbsorptionCount());
}

void UHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GetPlayerController()->OnViewCharacterChanged.AddUObject(this, &UHealthBarWidget::OnCharacterChanged);
	if (const ABaseCharacter* Character = GetPlayerController()->GetViewCharacter())
	{
		const auto StatComponent = Character->GetCharacterStatComponent();
		OnViewCharacterChangedHandle = StatComponent->OnHPChanged.AddUObject(this, &UHealthBarWidget::OnHPChanged);
		PB_Health->SetPercent(StatComponent->GetCurrentHp() / StatComponent->GetMaxHp());
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHealthBarWidget::OnCharacterChanged(ABaseCharacter* OldCharacter, ABaseCharacter* NewCharacter)
{
	if (OnViewCharacterChangedHandle.IsValid())
	{
		if (OldCharacter)
		{
			OldCharacter->GetCharacterStatComponent()->OnHPChanged.Remove(OnViewCharacterChangedHandle);
		}
		OnViewCharacterChangedHandle.Reset();
	}

	if (NewCharacter)
	{
		const auto StatComponent = NewCharacter->GetCharacterStatComponent();
		OnViewCharacterChangedHandle = StatComponent->OnHPChanged.AddUObject(this, &UHealthBarWidget::OnHPChanged);
		PB_Health->SetPercent(StatComponent->GetCurrentHp() / StatComponent->GetMaxHp());
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHealthBarWidget::OnHPChanged(float Current, float Max)
{
	ReturnIfTrue(Max == 0);
	PB_Health->SetPercent(Current / Max);
}
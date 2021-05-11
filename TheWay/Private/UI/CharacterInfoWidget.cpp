// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterInfoWidget.h"
#include "Utility/CreataekUtility.h"
#include "Controller/BasePlayerController.h"
#include "Character/BaseCharacter.h"
#include "Components/TextBlock.h"

void UCharacterInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GetPlayerController()->OnPickActor.AddUObject(this, &UCharacterInfoWidget::OnPickActor);
}

void UCharacterInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(TargetCharacter.IsValid() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UCharacterInfoWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateWidget();
}

void UCharacterInfoWidget::UpdateWidget()
{
	ReturnIfFalse(TargetCharacter.IsValid());

	Text_Name->SetText(FText::FromString(TargetCharacter->GetName()));
	Text_HP->SetText(FText::AsNumber(TargetCharacter->GetCharacterStatComponent()->GetCurrentHp()));
	Text_AD->SetText(FText::AsNumber(TargetCharacter->GetFinalAttckDamage()));
}

void UCharacterInfoWidget::OnPickActor(TWeakObjectPtr<AActor> PickedActor)
{
	TargetCharacter = Cast<ABaseCharacter>(PickedActor);

	SetVisibility(TargetCharacter.IsValid() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SpawnerInfoWidget.h"
#include "Utility/CreataekUtility.h"
#include "Controller/BasePlayerController.h"
#include "Spawner/CharacterSpawner.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "DataTable/TheWayDataTable.h"
#include "Framework/BasePlayerState.h"

void USpawnerInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_LevelUp->OnClicked.AddDynamic(this, &USpawnerInfoWidget::OnLevelUpClicked);
	GetPlayerController()->OnPickActor.AddUObject(this, &USpawnerInfoWidget::OnPickActor);
}

void USpawnerInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(TargetSpawner.IsValid() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void USpawnerInfoWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateWidget();
}

void USpawnerInfoWidget::OnLevelUpClicked()
{
	ReturnIfFalse(TargetSpawner.IsValid());

	ABasePlayerState* BasePlayerState = GetPlayerController()->GetPlayerState<ABasePlayerState>();
	ReturnIfNull(BasePlayerState);

	if (!TargetSpawner->IsMaxLevel() && BasePlayerState->TryConsumeSoul(TargetSpawner->GetSpawnerDataCsv()->SoulNumToNextLevel))
	{
		TargetSpawner->LevelUp();
	}
}

void USpawnerInfoWidget::UpdateWidget()
{
	ReturnIfFalse(TargetSpawner.IsValid());

	Text_Name->SetText(FText::FromString(TargetSpawner->GetName()));
	Text_Level->SetText(FText::AsNumber(TargetSpawner->GetSpawnerLevel()));
	Text_Interval->SetText(FText::AsNumber(TargetSpawner->GetSpawnInterval()));

	TArray<FText> SpawnProbabilityTextList;
	for (const FProbabilityData& Data : TargetSpawner->GetSpawnProbabilityList())
	{
		FText Format = FText::FromString("Actor ID : {0}");
		SpawnProbabilityTextList.Add(FText::Format(Format, Data.Id));
	}
	Text_SpawnProbabilityList->SetText(FText::Join(CU::NewLine(), SpawnProbabilityTextList));
	Text_LevelUpCost->SetText(FText::AsNumber(TargetSpawner->GetSpawnerDataCsv()->SoulNumToNextLevel));

	Button_LevelUp->SetVisibility(TargetSpawner->GetSpawnerDataCsv()->SoulNumToNextLevel > 0 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void USpawnerInfoWidget::OnPickActor(TWeakObjectPtr<AActor> PickedActor)
{
	TargetSpawner = Cast<ACharacterSpawner>(PickedActor);
	SetVisibility(TargetSpawner.IsValid() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}
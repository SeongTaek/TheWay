// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServices/BTService_GetCharacterStats.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utility/CreataekUtility.h"
#include "Controller/BaseAIController.h"
#include "Character/BaseCharacter.h"
#include "Item/Weapon/WeaponBase.h"
#include "Components/CharacterStatComponent.h"
#include "Components/WeaponStatComponent.h"

FString UBTService_GetCharacterStats::GetStaticDescription() const
{
	return FString::Printf(TEXT("선택된 블랙보드에 홈 위치(%s), 순찰 범위(%s), \n탐색 범위(%s), 공격 범위(%s) 를 설정합니다."),
		*BBKeyHomePos.SelectedKeyName.ToString(),
		*BBKeyPatrolRadius.SelectedKeyName.ToString(),
		*BBKeyDetectRadius.SelectedKeyName.ToString(),
		*BBKeyAttackRange.SelectedKeyName.ToString());
}

UBTService_GetCharacterStats::UBTService_GetCharacterStats()
{
	NodeName = TEXT("캐릭터 스탯 설정");

	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
	bNotifyTick = false;
}

void UBTService_GetCharacterStats::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	ABaseAIController* AIController = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	ReturnIfNull(ControllingPawn);

	ABaseCharacter* ControllingCharacter = Cast<ABaseCharacter>(ControllingPawn);
	ReturnIfNull(ControllingPawn);

	UWorld* World = ControllingCharacter->GetWorld();
	ReturnIfNull(World);

	UBlackboardComponent* BlackBoardComponent = OwnerComp.GetBlackboardComponent();
	const UCharacterStatComponent* CharacterStatComponent = ControllingCharacter->GetCharacterStatComponent();
	const UWeaponStatComponent* WeaponStatComponent = (ControllingCharacter->GetEquippedWeapon() ? ControllingCharacter->GetEquippedWeapon()->GetWeaponStatComponent() : nullptr);
	float AttackRange = (WeaponStatComponent ? WeaponStatComponent->GetAttackRange() : 0.0f);
	float PatrolRadius = (CharacterStatComponent ? CharacterStatComponent->GetPatrolRadius() : 1000.0f);
	float DetectRadius = (CharacterStatComponent ? CharacterStatComponent->GetDetectRadius() : 2000.0f);

	BlackBoardComponent->SetValueAsVector(BBKeyHomePos.SelectedKeyName, ControllingCharacter->GetActorLocation());
	BlackBoardComponent->SetValueAsFloat(BBKeyPatrolRadius.SelectedKeyName, PatrolRadius);
	BlackBoardComponent->SetValueAsFloat(BBKeyDetectRadius.SelectedKeyName, DetectRadius);
	BlackBoardComponent->SetValueAsFloat(BBKeyAttackRange.SelectedKeyName, AttackRange);

	AIController->NotifyEquippedWeaponChanged.AddUObject(this, &UBTService_GetCharacterStats::UpdateEquippedWeaponData);
	AIController->NotifyEquippedWeaponLevelUp.AddUObject(this, &UBTService_GetCharacterStats::UpdateEquippedWeaponData);
}

void UBTService_GetCharacterStats::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
}

void UBTService_GetCharacterStats::UpdateEquippedWeaponData(ABaseAIController* AIController)
{
	ReturnIfNull(AIController);

	UBlackboardComponent* BlackBoardComponent = AIController->GetBlackboardComponent();
	APawn* ControllingPawn = AIController->GetPawn();
	ReturnIfNull(ControllingPawn);

	ABaseCharacter* ControllingCharacter = Cast<ABaseCharacter>(ControllingPawn);
	ReturnIfNull(ControllingPawn);

	const float AttackRange = (ControllingCharacter->GetEquippedWeapon() != nullptr ? ControllingCharacter->GetEquippedWeapon()->GetWeaponStatComponent()->GetAttackRange() : 0.0f);
	BlackBoardComponent->SetValueAsFloat(BBKeyAttackRange.SelectedKeyName, AttackRange);
}

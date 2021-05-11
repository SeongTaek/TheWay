// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServices/BTService_CheckIsInAttackRange.h"
#include "Character/BaseCharacter.h"
#include "Controller/BaseAIController.h"
#include "Utility/CreataekUtility.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Item/Weapon/WeaponBase.h"
#include "Item/Weapon/RangedWeapon/RangedWeaponBase.h"
#include "Item/Weapon/MeleeWeapon/MeleeWeaponBase.h"

FString UBTService_CheckIsInAttackRange::GetStaticDescription() const
{
	return FString::Printf(TEXT("대상(%s)이 공격 범위(%s) 안에 있으면 Task를 성공시킵니다."), *BBKeyTarget.SelectedKeyName.ToString(), *BBKeyAttackRange.SelectedKeyName.ToString());
}

UBTService_CheckIsInAttackRange::UBTService_CheckIsInAttackRange()
{
	NodeName = TEXT("공격 범위 확인");
}

void UBTService_CheckIsInAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	ReturnIfFalse(ControllingPawn);

	ABaseCharacter* ControllingCharacter = Cast<ABaseCharacter>(ControllingPawn);
	ReturnIfFalse(ControllingCharacter);

	auto Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKeyTarget.SelectedKeyName));
	ReturnIfFalse(Target);

	if (Target->GetDistanceTo(ControllingCharacter) <= OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKeyAttackRange.SelectedKeyName))
	{
		OwnerComp.RequestExecution(EBTNodeResult::Succeeded);
	}
}
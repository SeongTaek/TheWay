// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_IsInAttackRange.h"
#include "Character/BaseCharacter.h"
#include "Controller/BaseAIController.h"
#include "Utility/CreataekUtility.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Item/Weapon/WeaponBase.h"
#include "Item/Weapon/RangedWeapon/RangedWeaponBase.h"
#include "Item/Weapon/MeleeWeapon/MeleeWeaponBase.h"

FString UBTDecorator_IsInAttackRange::GetStaticDescription() const
{
	return FString::Printf(TEXT("대상(%s)이 공격 범위(%s) 안에 있는지 확인합니다."), *BBKeyTarget.SelectedKeyName.ToString(), *BBKeyAttackRange.SelectedKeyName.ToString());
}

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("공격 범위 확인");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	ReturnFalseIfNull(ControllingPawn);

	ABaseCharacter* ControllingCharacter = Cast<ABaseCharacter>(ControllingPawn);
	ReturnFalseIfNull(ControllingCharacter);

	auto Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKeyTarget.SelectedKeyName));
	ReturnFalseIfNull(Target);

	// 공격 범위 안에 있는지 체크
	bResult = (Target->GetDistanceTo(ControllingCharacter) <= OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKeyAttackRange.SelectedKeyName));

	return bResult;
}

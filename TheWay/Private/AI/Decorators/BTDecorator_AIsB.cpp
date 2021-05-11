// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_AIsB.h"
#include "Controller/BaseAIController.h"
#include "Character/BaseCharacter.h"
#include "Utility/CreataekUtility.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_AIsB::UBTDecorator_AIsB()
{
	NodeName = TEXT("A Is Equal To B");
}

bool UBTDecorator_AIsB::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	ReturnFalseIfNull(ControllingPawn);

	auto A = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AKey));
	auto B = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BKey));

	bResult = A == B;
	return bResult;
}

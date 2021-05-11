// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_CheckObstacleBetween.h"
#include "Character/BaseCharacter.h"
#include "Controller/BaseAIController.h"
#include "Utility/CreataekUtility.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

FString UBTDecorator_CheckObstacleBetween::GetStaticDescription() const
{
	return FString::Printf(TEXT("대상(%s) 사이에 장애물이 있는지 확인합니다."), *BBKeyTarget.SelectedKeyName.ToString());
}

UBTDecorator_CheckObstacleBetween::UBTDecorator_CheckObstacleBetween()
{
	NodeName = TEXT("대상 사이에 장애물 확인");
}

bool UBTDecorator_CheckObstacleBetween::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	ReturnFalseIfNull(ControllingPawn);

	ABaseCharacter* ControllingCharacter = Cast<ABaseCharacter>(ControllingPawn);
	ReturnFalseIfNull(ControllingCharacter);

	auto Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKeyTarget.SelectedKeyName));
	ReturnFalseIfNull(Target);

	// 타겟 사이에 장애물이 있는지 체크
	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	ECollisionChannel TraceChannel = ControllingCharacter->GetProjectileCollisionChannel();

	GetWorld()->LineTraceSingleByChannel(Hit, ControllingCharacter->GetActorLocation(), Target->GetActorLocation(), TraceChannel, CollisionParams);

	bResult = (Hit.GetActor() != nullptr && Hit.GetActor() != Target && Cast<ABaseCharacter>(Hit.GetActor()) == nullptr);
	return bResult;
}

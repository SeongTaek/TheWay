// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/BTTask_FindPatrolPos.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Controller/BaseAIController.h"

FString UBTTask_FindPatrolPos::GetStaticDescription() const
{
	return FString::Printf(TEXT("현재 위치에서 %s 반경 랜덤 위치를 %s에 설정합니다."), *BBKeyPatrolRadius.SelectedKeyName.ToString(), *BBKeyPatrolPos.SelectedKeyName.ToString());
	//return FString::Printf(TEXT("%s 중심으로 %s 반경 랜덤 위치를 %s에 설정합니다."), *BBKeyHomePos.SelectedKeyName.ToString(), *BBKeyPatrolRadius.SelectedKeyName.ToString(), *BBKeyPatrolPos.SelectedKeyName.ToString());
}

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("순찰하기");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem)
	{
		return EBTNodeResult::Failed;
	}

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKeyHomePos.SelectedKeyName);
	FNavLocation NextPatrol;

	//if (NavSystem->GetRandomPointInNavigableRadius(Origin, OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKeyPatrolRadius.SelectedKeyName), NextPatrol))
	if (NavSystem->GetRandomPointInNavigableRadius(ControllingPawn->GetActorLocation(), OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKeyPatrolRadius.SelectedKeyName), NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKeyPatrolPos.SelectedKeyName, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

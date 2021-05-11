// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PlayerAIController.h"
#include "Utility/CreataekUtility.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BaseCharacter.h"

APlayerAIController::APlayerAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/Characters/ParagonGideon/Characters/Heroes/Gideon/AI/BB_GideonAI.BB_GideonAI'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/Characters/ParagonGideon/Characters/Heroes/Gideon/AI/BT_GideonAI.BT_GideonAI'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void APlayerAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector("HomePos", InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
		}
	}
}

void APlayerAIController::OnUnPossess()
{
	Super::OnUnPossess();
}



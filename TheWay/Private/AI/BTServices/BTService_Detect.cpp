// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServices/BTService_Detect.h"
#include "Character/BaseCharacter.h"
#include "Controller/BaseAIController.h"
#include "Utility/CreataekUtility.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

FString UBTService_Detect::GetStaticDescription() const
{
	return FString::Printf(TEXT("선택된 블랙보드 변수(%s)에 범위(%s) 안에서 가장 어그로가 높은 적을 설정합니다."), *BBKeyTarget.SelectedKeyName.ToString(), *BBKeyDetectRadius.SelectedKeyName.ToString());
}

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("목표물 탐색하기");

	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
	bNotifyTick = true;

	Interval = 1.0f;
}

void UBTService_Detect::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void UBTService_Detect::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	ReturnIfNull(ControllingPawn);

	ABaseCharacter* ControllingCharacter = Cast<ABaseCharacter>(ControllingPawn);
	ReturnIfNull(ControllingPawn);

	UWorld* World = ControllingCharacter->GetWorld();
	FVector Center = ControllingCharacter->GetActorLocation();
	ReturnIfNull(World);

	const float DetectRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKeyDetectRadius.SelectedKeyName);

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingCharacter);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ControllingCharacter->GetAttackCollisionChannel(),
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam);

	if (bResult)
	{
		ABaseCharacter* AggressiveCharacter = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKeyTarget.SelectedKeyName));
		if (AggressiveCharacter && AggressiveCharacter->IsDead())
		{
			AggressiveCharacter = nullptr;
		}

		int32 MaxAggroPoint = (AggressiveCharacter ? AggressiveCharacter->GetAggroPoint(ControllingCharacter) : -1);

		for (FOverlapResult OverlapResult : OverlapResults)
		{
			ABaseCharacter* Character = Cast<ABaseCharacter>(OverlapResult.GetActor());

			if (Character && ControllingCharacter->GetTeamAttitudeTowards(*Character) != ETeamAttitude::Friendly)
			{
				int NewAggroPoint = Character->GetAggroPoint(ControllingCharacter);
				if (MaxAggroPoint < NewAggroPoint)
				{
					MaxAggroPoint = NewAggroPoint;
					AggressiveCharacter = Character;
					//DrawDebugLine(World, ControllingPawn->GetActorLocation(), Character->GetActorLocation(), FColor::Blue, false, 0.2f);
				}
			}
		}

		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKeyTarget.SelectedKeyName, AggressiveCharacter);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKeyTarget.SelectedKeyName, nullptr);
	}

	//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}

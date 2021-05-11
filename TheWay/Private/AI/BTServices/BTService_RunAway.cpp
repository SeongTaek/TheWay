// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTServices/BTService_RunAway.h"
#include "Character/BaseCharacter.h"
#include "Controller/BaseAIController.h"
#include "Utility/CreataekUtility.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Math/NumericLimits.h"

FString UBTService_RunAway::GetStaticDescription() const
{
	if (DistanceScale == 1.0f)
	{
		return FString::Printf(TEXT("거리(%s)를 유지하며 도망 위치(%s)를 구합니다."), *BBKeyKeepDistance.SelectedKeyName.ToString(), *BBKeyRunAwayPos.SelectedKeyName.ToString());
	}
	else
	{
		return FString::Printf(TEXT("거리(%s %s)를 유지하며 도망 위치(%s)를 구합니다."), *BBKeyKeepDistance.SelectedKeyName.ToString(), *FText::AsPercent(DistanceScale).ToString(), *BBKeyRunAwayPos.SelectedKeyName.ToString());
	}
}

UBTService_RunAway::UBTService_RunAway()
{
	NodeName = TEXT("대상에게서 도망갈 위치 구하기");

	DistanceScale = 1.0f;
}

void UBTService_RunAway::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void UBTService_RunAway::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
}

void UBTService_RunAway::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	ReturnIfFalse(ControllingPawn);

	ABaseCharacter* ControllingCharacter = Cast<ABaseCharacter>(ControllingPawn);
	ReturnIfFalse(ControllingCharacter);

	auto Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKeyTarget.SelectedKeyName));
	ReturnIfFalse(Target);

	float KeepDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKeyKeepDistance.SelectedKeyName) * DistanceScale;
	FVector BestRunAwayPoint = CalculateBestRunAwayPoint(ControllingCharacter, Target, KeepDistance);

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKeyRunAwayPos.SelectedKeyName, BestRunAwayPoint);
}

FVector UBTService_RunAway::CalculateBestRunAwayPoint(const ABaseCharacter* OwnerCharacter, const ABaseCharacter* TargetCharacter, const float RunAwayDistance)
{
	if (OwnerCharacter == nullptr || TargetCharacter == nullptr)
	{
		return FVector::ZeroVector;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(OwnerCharacter->GetWorld());
	if (NavSystem == nullptr)
	{
		return OwnerCharacter->GetActorLocation();
	}

	const float DistanceFromTarget = FVector::Distance(OwnerCharacter->GetActorLocation(), TargetCharacter->GetActorLocation());
	FVector OwnerForwardVector = OwnerCharacter->GetActorForwardVector();
	const int LineTraceNum = 18;
	const float RotateAngle = (360.0f / (float)LineTraceNum);
	int AggroPoints[LineTraceNum]{ 0, };
	int MinAggroPoint = TNumericLimits<int>::Max();

	// RotateAngle 단위로 LineTraceNum 방향 검사해서 어그로 요소 체크
	for (int i = 0; i < LineTraceNum; ++i)
	{
		TArray<FHitResult> Hits;
		FCollisionQueryParams CollisionParams;
		ECollisionChannel TraceChannel = OwnerCharacter->GetProjectileCollisionChannel();
		FVector LineTraceDirection = OwnerForwardVector.RotateAngleAxis(RotateAngle * (float)i, FVector::ZAxisVector);
		FNavLocation RunAwayPoint;

		if (NavSystem->ProjectPointToNavigation(OwnerCharacter->GetActorLocation() + LineTraceDirection * RunAwayDistance, RunAwayPoint))
		{
			GetWorld()->LineTraceMultiByChannel(Hits, OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorLocation() + LineTraceDirection * RunAwayDistance, TraceChannel, CollisionParams);

			for (FHitResult& Hit : Hits)
			{
				if (Cast<ABaseCharacter>(Hit.GetActor()) != nullptr)
				{
					int DistanceFromHitActor = FVector::Distance(OwnerCharacter->GetActorLocation(), Hit.GetActor()->GetActorLocation());
					AggroPoints[i] += DistanceFromHitActor;
				}
			}

			if (MinAggroPoint > AggroPoints[i])
			{
				MinAggroPoint = AggroPoints[i];
			}
		}
		else
		{
			AggroPoints[i] = TNumericLimits<int>::Max();
		}
	}
	// 어그로 점수가 가장 낮은 위치를 찾는다.
	TArray<FVector> BestRunAwayDirectionsInForward;	// 타겟 방향
	TArray<FVector> BestRunAwayDirectionsInBackward;	// 타겟 반대 방향

	for (int i = 0; i < LineTraceNum; ++i)
	{
		FVector Direction = OwnerForwardVector.RotateAngleAxis(RotateAngle * (float)i, FVector::ZAxisVector).GetSafeNormal();

		if (AggroPoints[i] == MinAggroPoint)
		{
			if (FVector::DotProduct(Direction, OwnerForwardVector) > 0)
			{
				BestRunAwayDirectionsInForward.Add(Direction);
			}
			else
			{
				BestRunAwayDirectionsInBackward.Add(Direction);
			}
		}
	}

	// 도망가기에 유리한 타겟 반대 방향 먼저 체크
	while (BestRunAwayDirectionsInBackward.Num() > 0)
	{
		FNavLocation RunAwayPoint;
		int idx = FMath::RandRange(0, BestRunAwayDirectionsInBackward.Num() - 1);
		FVector Direction = BestRunAwayDirectionsInBackward[idx];

		if (NavSystem->ProjectPointToNavigation(OwnerCharacter->GetActorLocation() + Direction * (RunAwayDistance - DistanceFromTarget), RunAwayPoint))
		{
			return RunAwayPoint.Location;
		}

		BestRunAwayDirectionsInBackward.RemoveAt(idx);
	}

	// 반대 방향에 유효한 값이 없으면 타겟 방향 체크
	while (BestRunAwayDirectionsInForward.Num() > 0)
	{
		FNavLocation RunAwayPoint;
		int idx = FMath::RandRange(0, BestRunAwayDirectionsInForward.Num() - 1);
		FVector Direction = BestRunAwayDirectionsInForward[idx];

		if (NavSystem->ProjectPointToNavigation(OwnerCharacter->GetActorLocation() + Direction * (RunAwayDistance - DistanceFromTarget), RunAwayPoint))
		{
			return RunAwayPoint.Location;
		}

		BestRunAwayDirectionsInForward.RemoveAt(idx);
	}

	return OwnerCharacter->GetActorLocation();
}

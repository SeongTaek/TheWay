// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_RunAway.generated.h"

/**
 * 
 */
UCLASS()
class UBTService_RunAway : public UBTService
{
	GENERATED_BODY()

private:
	virtual FString GetStaticDescription() const override;

public:
	UBTService_RunAway();

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	FVector CalculateBestRunAwayPoint(const class ABaseCharacter* OwnerCharacter, const class ABaseCharacter* TargetCharacter, const float RunAwayDistance);

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BBKeyTarget;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BBKeyKeepDistance;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BBKeyRunAwayPos;

	UPROPERTY(EditAnywhere, Category = Factors)
	float DistanceScale;
};
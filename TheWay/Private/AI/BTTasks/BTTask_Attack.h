// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()

private:
	virtual FString GetStaticDescription() const;

public:
	UBTTask_Attack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BBKeyTarget;

	UPROPERTY(EditAnywhere, Category = TurnToTarget)
	bool bTurnToTarget = true;

	UPROPERTY(EditAnywhere, Category = TurnToTarget)
	float TurnSpeed = 10.0f;

private:
	bool IsAttacking = false;
};

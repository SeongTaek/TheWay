// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPatrolPos.generated.h"

/**
 * 
 */
UCLASS()
class UBTTask_FindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()

private:
	virtual FString GetStaticDescription() const;

public:
	UBTTask_FindPatrolPos();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BBKeyHomePos;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BBKeyPatrolPos;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BBKeyPatrolRadius;
};

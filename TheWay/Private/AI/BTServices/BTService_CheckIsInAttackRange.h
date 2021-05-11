// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckIsInAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class UBTService_CheckIsInAttackRange : public UBTService
{
	GENERATED_BODY()

private:
	virtual FString GetStaticDescription() const;

public:
	UBTService_CheckIsInAttackRange();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BBKeyTarget;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BBKeyAttackRange;
};

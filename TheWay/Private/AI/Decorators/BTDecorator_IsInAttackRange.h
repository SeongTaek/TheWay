// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsInAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class UBTDecorator_IsInAttackRange : public UBTDecorator
{
	GENERATED_BODY()
	
private:
	virtual FString GetStaticDescription() const override;

public:
	UBTDecorator_IsInAttackRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BBKeyTarget;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BBKeyAttackRange;
};

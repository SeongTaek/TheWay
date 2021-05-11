// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckObstacleBetween.generated.h"

/**
 * 
 */
UCLASS()
class UBTDecorator_CheckObstacleBetween : public UBTDecorator
{
	GENERATED_BODY()

private:
	virtual FString GetStaticDescription() const override;

public:
	UBTDecorator_CheckObstacleBetween();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BBKeyTarget;
};

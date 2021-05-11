// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_AIsB.generated.h"

/**
 *
 */
UCLASS()
class UBTDecorator_AIsB : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_AIsB();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere)
	FName AKey;

	UPROPERTY(EditAnywhere)
	FName BKey;
};

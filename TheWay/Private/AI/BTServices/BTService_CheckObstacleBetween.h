// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckObstacleBetween.generated.h"

/**
 * 
 */
UCLASS()
class UBTService_CheckObstacleBetween : public UBTService
{
	GENERATED_BODY()

private:
	virtual FString GetStaticDescription() const override;

public:
	UBTService_CheckObstacleBetween();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BBKeyTarget;
};

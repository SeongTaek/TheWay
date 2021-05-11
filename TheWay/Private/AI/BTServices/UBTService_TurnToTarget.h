// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "UBTService_TurnToTarget.generated.h"

/**
 * 
 */
UCLASS()
class UUBTService_TurnToTarget : public UBTService
{
	GENERATED_BODY()

private:
	virtual FString GetStaticDescription() const;

public:
	UUBTService_TurnToTarget();

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BBKeyTarget;
	
};

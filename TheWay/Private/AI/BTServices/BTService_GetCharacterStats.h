// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_GetCharacterStats.generated.h"

/**
 * 
 */
UCLASS()
class UBTService_GetCharacterStats : public UBTService
{
	GENERATED_BODY()
	
private:
	virtual FString GetStaticDescription() const override;

public:
	UBTService_GetCharacterStats();

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UFUNCTION()
	void UpdateEquippedWeaponData(class ABaseAIController* BaseAIController);

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BBKeyHomePos;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BBKeyPatrolRadius;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BBKeyDetectRadius;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BBKeyAttackRange;

};

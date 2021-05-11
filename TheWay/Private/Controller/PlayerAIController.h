// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/BaseAIController.h"
#include "PlayerAIController.generated.h"

/**
 * 
 */
UCLASS()
class APlayerAIController : public ABaseAIController
{
	GENERATED_BODY()

public:
	APlayerAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;
};

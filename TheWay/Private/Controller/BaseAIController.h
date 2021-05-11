// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FNotifyAIControllerDataChanged, class ABaseAIController*);

/**
 * 
 */
UCLASS()
class ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABaseAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	void RestartAI();
	void StopAI();

protected:
	UFUNCTION()
	void OnEquippedWeaponChanged(class AWeaponBase* LastWeapon, class AWeaponBase* EquippedWeapon);
	UFUNCTION()
	void OnEquippedWeaponLevelUp();

public:
	FNotifyAIControllerDataChanged NotifyEquippedWeaponChanged;
	FNotifyAIControllerDataChanged NotifyEquippedWeaponLevelUp;

protected:
	FDelegateHandle OnEquippedWeaponLevelUpHandle;
};

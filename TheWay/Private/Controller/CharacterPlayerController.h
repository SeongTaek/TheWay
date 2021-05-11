// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/BasePlayerController.h"
#include "CharacterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ACharacterPlayerController : public ABasePlayerController
{
	GENERATED_BODY()

public:
	ACharacterPlayerController();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void SetupInputComponent() override;

protected:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Turn(float AxisValue);
	void LookUp(float AxisValue);
	void Attack();
	void OpenPausePopup();

	virtual void OnViewCharacterDeath();

	UFUNCTION()
	void TimerHandle_SearchHostileCharacter();

protected:
	UPROPERTY(EditDefaultsOnly)
	float SearchHostileCharacterInterval = 0.5f;

	FTimerHandle SearchHostileCharacterTimer;

	bool bPausePopupOpend = false;
};

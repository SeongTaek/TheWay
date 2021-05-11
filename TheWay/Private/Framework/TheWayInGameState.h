// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/BaseGameStateBase.h"
#include "TheWayInGameState.generated.h"

/**
 * 
 */
UCLASS()
class ATheWayInGameState : public ABaseGameStateBase
{
	GENERATED_BODY()
	
public:
	ATheWayInGameState();

	void NoticeWaveMessage(const int32 Wave);
	class UInGameFinishPopup* OpenGameFinishPopup();
	class UGamePausePopup* OpenGamePausePopup();

	int32 GetCurrentWave() const { return CurrentWave; }

protected:
	int32 CurrentWave;
};

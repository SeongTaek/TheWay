// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mode/TheWayGameModeBase.h"
#include "TheWayInGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ATheWayInGameMode : public ATheWayGameModeBase
{
	GENERATED_BODY()

public:
	enum EGamePhase : uint8
	{
		NONE,
		PLAYING_SETUP,
		PLAYING_WAVE,
		RESULT,
		END,
	};
	
public:
	ATheWayInGameMode();

public:
	virtual void StartPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void InitGameState() override;
	virtual void Tick(float DeltaSeconds) override;

	void ChangePhase(EGamePhase ePhase);

protected:
	UFUNCTION()
	void TimerHandle_SpawnCharacterTime();

	void RegisterPhaseLogic(EGamePhase ePhase, class TheWayInGamePhaseLogic* Logic);
	void PrepareWaveData();
	void UpdateCharacterData();

public:
	UPROPERTY(EditAnywhere);
	class UDataTable* WaveTable;

	TArray<const struct FInGameWaveData*> Waves;
	TArray<const class ABaseCharacter*> FriendlyCharacters;

protected:
	class ATheWayInGameState* InGameState;

	TArray<TSharedPtr<class TheWayInGamePhaseLogic>> PhaseLogics;
	TWeakPtr<class TheWayInGamePhaseLogic> CurrentPhaseLigic;

	FTimerHandle SpawnCharacterTimer;
};

class TheWayInGamePhaseLogic
{
public:
	ATheWayInGameMode::EGamePhase ePhase;
	ATheWayInGameMode* GameMode;
	class ATheWayInGameState* GameState;
	UWorld* World;

	virtual ~TheWayInGamePhaseLogic() {}

	virtual void OnEnter() { PhaseStartTIme = World->GetTimeSeconds(); }
	virtual void OnTick(float DeltaSeconds) {}
	virtual void OnExit() {}

	float GetPhaseElapsedTime() const { return World->GetTimeSeconds() - PhaseStartTIme; }

	float PhaseStartTIme;
};

class TheWayInGamePhase_PlayingSetup : public TheWayInGamePhaseLogic
{
public:
	virtual void OnEnter() override;
	virtual void OnTick(float DeltaSeconds) override;
	virtual void OnExit() override;

protected:
	void WaitPlayer();
};

class TheWayInGamePhase_PlayingWave : public TheWayInGamePhaseLogic
{
public:
	virtual void OnEnter() override;
	virtual void OnTick(float DeltaSeconds) override;
	virtual void OnExit() override;

protected:
	void ProcessWave();
	void CheckClear();
	void CheckGameOver();

protected:
	int32 WaveBeginIndex;
	int32 WaveEndIndex;
	int32 CurrentWaveIndex;
};

class TheWayInGamePhase_Result : public TheWayInGamePhaseLogic
{
public:
	virtual void OnEnter() override;
	virtual void OnTick(float DeltaSeconds) override;
	virtual void OnExit() override;
};

class TheWayInGamePhase_End : public TheWayInGamePhaseLogic
{
public:
	virtual void OnEnter() override;
	virtual void OnTick(float DeltaSeconds) override;
	virtual void OnExit() override;
};


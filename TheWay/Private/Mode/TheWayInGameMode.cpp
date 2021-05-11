// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/TheWayInGameMode.h"
#include "EngineUtils.h"
#include "Framework/TheWayInGameState.h"
#include "Utility/CreataekUtility.h"
#include "DataTable/TheWayDataTable.h"
#include "Character/BaseCharacter.h"
#include "Controller/BasePlayerController.h"
#include "Framework/BasePlayerState.h"
#include "Framework/BaseGameStateBase.h"
#include "UI/InGameHUD.h"
#include "DataTable/DataTableManager.h"
#include "DataTable/TheWayDataTable.h"
#include "NavigationSystem.h"
#include "Spawner/CharacterSpawner.h"
#include "Kismet/GameplayStatics.h"

ATheWayInGameMode::ATheWayInGameMode()
{
	DefaultPawnClass = nullptr;
	PlayerControllerClass = ABasePlayerController::StaticClass();
	PlayerStateClass = ABasePlayerState::StaticClass();
	GameStateClass = ABaseGameStateBase::StaticClass();
	HUDClass = AInGameHUD::StaticClass();
}

void ATheWayInGameMode::StartPlay()
{
	Super::StartPlay();

	RegisterPhaseLogic(EGamePhase::PLAYING_SETUP, new TheWayInGamePhase_PlayingSetup);
	RegisterPhaseLogic(EGamePhase::PLAYING_WAVE, new TheWayInGamePhase_PlayingWave);
	RegisterPhaseLogic(EGamePhase::RESULT, new TheWayInGamePhase_Result);
	RegisterPhaseLogic(EGamePhase::END, new TheWayInGamePhase_End);

	PrepareWaveData();

	// 맵에 미리 스폰해놓은 액터는 ID 값이 설정되지 않아서 게임 시작 시 검색해서 ID를 설정해준다.
	// 추후 서버에서 스폰 패킷을 전송하면 클라에서 ID 설정해주는 로직 제거 예정!
	TArray<AActor*> Spawners;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterSpawner::StaticClass(), Spawners);
	for (AActor* Spawner : Spawners)
	{
		Cast<ACharacterSpawner>(Spawner)->SetSpawnerID(TPair<int32, int32>(10001, 1));
	}

	// 2초 후에 플레이어 캐릭터 스폰하기
	GetWorld()->GetTimerManager().SetTimer(SpawnCharacterTimer, this, &ATheWayInGameMode::TimerHandle_SpawnCharacterTime, 2.0f, false);

	ChangePhase(EGamePhase::PLAYING_SETUP);
}

void ATheWayInGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ATheWayInGameMode::InitGameState()
{
	Super::InitGameState();
	InGameState = GetGameState<ATheWayInGameState>();
}

void ATheWayInGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateCharacterData();

	if (CurrentPhaseLigic.IsValid())
	{
		CurrentPhaseLigic.Pin()->OnTick(DeltaSeconds);
	}
}

void ATheWayInGameMode::TimerHandle_SpawnCharacterTime()
{
	if (auto CharacterData = UDataTableManager::Get(this)->GetTableCsv<FCharacterData>(1000001))
	{
		ABaseCharacter* Character = ABaseCharacter::Spawn(this, CharacterData, FVector(-2600.0f, -1400.0f, 185.0f), FQuat::Identity);

		if (auto BasePlayerController = Cast<ABasePlayerController>(GetWorld()->GetFirstPlayerController()))
		{
			BasePlayerController->Possess(Character);
			BasePlayerController->SetViewCharacater(Character);
			CU::DelayExecute(GetWorld(), [=]
			{
				ChangePhase(EGamePhase::PLAYING_WAVE);
			}, 2.0f);
		}
	}
}

void ATheWayInGameMode::RegisterPhaseLogic(EGamePhase ePhase, TheWayInGamePhaseLogic* Logic)
{
	PhaseLogics.SetNum(FMath::Max(PhaseLogics.Num(), (int)ePhase + 1));
	PhaseLogics[(int)ePhase] = MakeShareable(Logic);

	Logic->ePhase = ePhase;
	Logic->GameMode = this;
	Logic->GameState = InGameState;
	Logic->World = GetWorld();
}

void ATheWayInGameMode::ChangePhase(EGamePhase ePhase)
{
	if (CurrentPhaseLigic != PhaseLogics[(int)ePhase])
	{
		if (CurrentPhaseLigic.IsValid())
		{
			CurrentPhaseLigic.Pin()->OnExit();
		}

		CurrentPhaseLigic = PhaseLogics[(int)ePhase];

		if (CurrentPhaseLigic.IsValid())
		{
			CurrentPhaseLigic.Pin()->OnEnter();
		}
	}
}

void ATheWayInGameMode::PrepareWaveData()
{
	ReturnIfNull(WaveTable);

	Waves.Reserve(WaveTable->GetRowMap().Num());

	for (auto& Pair : WaveTable->GetRowMap())
	{
		const FInGameWaveData* WaveData = (FInGameWaveData*)Pair.Value;
		Waves.Add(WaveData);
	}
}

void ATheWayInGameMode::UpdateCharacterData()
{
	FriendlyCharacters.Reset();
	for (TActorIterator<ABaseCharacter> It(GetWorld()); It; ++It)
	{
		ABaseCharacter* Character = *It;
		if (Character->GetGenericTeamId() == ETeamAttitude::Friendly)
		{
			FriendlyCharacters.Add(Character);
		}
	}
}

void TheWayInGamePhase_PlayingSetup::OnEnter()
{
	TheWayInGamePhaseLogic::OnEnter();
}

void TheWayInGamePhase_PlayingSetup::OnTick(float DeltaSeconds)
{
	TheWayInGamePhaseLogic::OnTick(DeltaSeconds);

	WaitPlayer();
}

void TheWayInGamePhase_PlayingSetup::OnExit()
{
	TheWayInGamePhaseLogic::OnExit();
}

void TheWayInGamePhase_PlayingSetup::WaitPlayer()
{
	// 1명 이상의 캐릭터가 생성되면 웨이브로 넘어간다.
	if (GameMode->FriendlyCharacters.Num())
	{
		GameMode->ChangePhase(ATheWayInGameMode::PLAYING_WAVE);
	}
}

void TheWayInGamePhase_PlayingWave::OnEnter()
{
	TheWayInGamePhaseLogic::OnEnter();

	WaveBeginIndex = 0;
	WaveEndIndex = GameMode->Waves.Num();
	CurrentWaveIndex = 0;
}

void TheWayInGamePhase_PlayingWave::OnTick(float DeltaSeconds)
{
	TheWayInGamePhaseLogic::OnTick(DeltaSeconds);

	ProcessWave();

	CheckClear();
	CheckGameOver();
}

void TheWayInGamePhase_PlayingWave::OnExit()
{
	TheWayInGamePhaseLogic::OnExit();
}

void TheWayInGamePhase_PlayingWave::ProcessWave()
{
	const float WaveTime = GetPhaseElapsedTime();
	float WaveEventTime = 0.0f;

	for (int32 WaveIndex = CurrentWaveIndex; WaveIndex <= WaveEndIndex; ++WaveIndex)
	{
		const FInGameWaveData* WaveData = GameMode->Waves.IsValidIndex(WaveIndex) ? GameMode->Waves[WaveIndex] : nullptr;
		if (WaveData == nullptr)
		{
			break;
		}

		WaveEventTime += WaveData->Time;
		if (WaveTime < WaveEventTime)
		{
			break;
		}

		++CurrentWaveIndex;

		GameState->NoticeWaveMessage(CurrentWaveIndex);

		// 필드에 생성된 스포너 레벨을 설정
		TArray<AActor*> Spawners;
		UGameplayStatics::GetAllActorsOfClass(World, ACharacterSpawner::StaticClass(), Spawners);
		if (Spawners.Num() == 0)
		{
			continue;
		}

		for (AActor* Spawner : Spawners)
		{
			Cast<ACharacterSpawner>(Spawner)->SetLevel(WaveData->SpawnerLevel);
		}

		if (UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(World))
		{
			// 보스가 있으면 스폰
			if (WaveData->BossCharacterId > 0)
			{
				ACharacterSpawner* BossSpawner = Cast<ACharacterSpawner>(Spawners[FMath::RandRange(0, Spawners.Num() - 1)]);
				BossSpawner->SpawnCharacter(WaveData->BossCharacterId);
			}

			// 아군 있으면 스폰
			if (auto BasePlayerController = Cast<ABasePlayerController>(World->GetFirstPlayerController()))
			{
				for (int32 FriendlyCharacterId : WaveData->FriendlyCharacterIdList)
				{
					FNavLocation SpawnLocation;
					if (NavSystem->GetRandomPointInNavigableRadius(BasePlayerController->GetPawn()->GetActorLocation(), 500.0f, SpawnLocation))
					{
						if (auto CharacterData = UDataTableManager::Get(GameMode)->GetTableCsv<FCharacterData>(FriendlyCharacterId))
						{
							ABaseCharacter* Character = ABaseCharacter::Spawn(GameMode, CharacterData, SpawnLocation, FQuat::Identity);
							Character->SetGenericTeamId(FGenericTeamId(ETeamAttitude::Friendly));
						}
					}
				}
			}

			// 적군 있으면 스폰
			for (int32 HostileCharacterId : WaveData->HostileCharacterIdList)
			{
				ACharacterSpawner* HostileCharacterSpawner = Cast<ACharacterSpawner>(Spawners[FMath::RandRange(0, Spawners.Num() - 1)]);
				HostileCharacterSpawner->SpawnCharacter(HostileCharacterId);
			}
		}
	}
}

void TheWayInGamePhase_PlayingWave::CheckClear()
{
}

void TheWayInGamePhase_PlayingWave::CheckGameOver()
{
	int32 DeadCharacterNum = 0;

	for (const ABaseCharacter* Character : GameMode->FriendlyCharacters)
	{
		if (Character->IsDead())
		{
			++DeadCharacterNum;
		}
	}

	if (DeadCharacterNum == GameMode->FriendlyCharacters.Num())
	{
		GameMode->ChangePhase(ATheWayInGameMode::RESULT);
	}
}

void TheWayInGamePhase_Result::OnEnter()
{
	TheWayInGamePhaseLogic::OnEnter();
}

void TheWayInGamePhase_Result::OnTick(float DeltaSeconds)
{
	TheWayInGamePhaseLogic::OnTick(DeltaSeconds);

	// 현재 Result 에서 처리할게 없어서 바로 End 로 변경한다.
	GameMode->ChangePhase(ATheWayInGameMode::END);
}

void TheWayInGamePhase_Result::OnExit()
{
	TheWayInGamePhaseLogic::OnExit();
}

void TheWayInGamePhase_End::OnEnter()
{
	TheWayInGamePhaseLogic::OnEnter();

	GameState->OpenGameFinishPopup();
}

void TheWayInGamePhase_End::OnTick(float DeltaSeconds)
{
	TheWayInGamePhaseLogic::OnTick(DeltaSeconds);
}

void TheWayInGamePhase_End::OnExit()
{
	TheWayInGamePhaseLogic::OnExit();
}

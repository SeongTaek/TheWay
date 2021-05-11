// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/CharacterSpawner.h"
#include "Utility/CreataekUtility.h"
#include "Framework/TheWayDeveloperSettings.h"
#include "Character/BaseCharacter.h"
#include "DataTable/DataTableManager.h"
#include "DataTable/TheWayDataTable.h"
#include "NavigationSystem.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"

ACharacterSpawner* ACharacterSpawner::Spawn(AActor* Owner, TSharedPtr<const struct FSpawnerData> SpawnerData)
{
	ReturnNullptrIfFalse(SpawnerData.IsValid());

	UClass* GeneratedBP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *CU::MakeClassPath(SpawnerData->Blueprint_File, &GetDefault<UTheWayDeveloperSettings>()->SpawnerClassBasePath)));
	UWorld* World = Owner->GetWorld();
	FActorSpawnParameters SpawnParams;
	FTransform SpawnTransform;

	SpawnParams.Owner = Owner;
	SpawnParams.Instigator = Owner->GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	SpawnParams.bDeferConstruction = true;
	SpawnTransform.SetLocation(Owner->GetActorLocation());
	SpawnTransform.SetRotation(Owner->GetActorRotation().Quaternion());

	ACharacterSpawner* Spawner = World->SpawnActor<ACharacterSpawner>(GeneratedBP, SpawnParams);
	Spawner->SetSpawnerID(TPair<int32, int32>(SpawnerData->Id, SpawnerData->Level), true);
	Spawner->FinishSpawning(SpawnTransform);

	return Spawner;
}

// Sets default values
ACharacterSpawner::ACharacterSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spawner"));
	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void ACharacterSpawner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACharacterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateSpawn(DeltaTime);
}

bool ACharacterSpawner::IsMaxLevel() const
{
	return SpawnerDataCsv->SoulNumToNextLevel == 0;
}

void ACharacterSpawner::LevelUp()
{
	ReturnIfTrue(IsMaxLevel());

	SetSpawnerID(TPair<int32, int32>(SpawnerId.Key, SpawnerId.Value + 1), true);
}

void ACharacterSpawner::SetLevel(int32 Level)
{
	ReturnIfTrue(IsMaxLevel());
	ReturnIfNull(UDataTableManager::Get(this));

	TPair<int32, int32> NewSpawnerID(SpawnerId.Key, Level);
	ReturnIfFalse(UDataTableManager::Get(this)->HasTableCsv<FSpawnerData>(NewSpawnerID));

	SetSpawnerID(NewSpawnerID, true);
}

void ACharacterSpawner::SetSpawnerID(TPair<int32, int32> Id, bool bUpdateProperties)
{
	SpawnerId = Id;
	CacheSpawnerCsv();

	if (bUpdateProperties && SpawnerDataCsv)
	{
		SpawnerLevel = SpawnerDataCsv->Level;
		SpawnInterval = SpawnerDataCsv->Interval;
		SpawnAreaRadius = SpawnerDataCsv->AreaRadius;
		MaxSpawnNum = SpawnerDataCsv->MaxNum;
		SpawnProbabilityList = SpawnerDataCsv->SpawnProbabilityList;
	}
}

void ACharacterSpawner::UpdateSpawn(const float DeltaTime)
{
	SpawnTimer += DeltaTime;
	ReturnIfFalse(SpawnTimer >= SpawnInterval);
	SpawnTimer -= SpawnInterval;

	RemoveDestroyedCharacters();
	ReturnIfFalse(SpawnedCharacters.Num() < MaxSpawnNum);

	SpawnCharacter();
}

void ACharacterSpawner::SpawnCharacter()
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	ReturnIfNull(NavSystem);

	// È®·ü µ¹¸®±â
	TArray<int> ToSpawnIdxList;
	for (int i = 0; i < SpawnProbabilityList.Num(); ++i)
	{
		if (FMath::RandRange(1, 100) < SpawnProbabilityList[i].Probability)
		{
			ToSpawnIdxList.Add(SpawnProbabilityList[i].Id);
		}
	}

	for (int i = 0; i < ToSpawnIdxList.Num(); ++i)
	{
		SpawnCharacter(ToSpawnIdxList[i]);
	}
}

void ACharacterSpawner::SpawnCharacter(int32 CharacterId)
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	ReturnIfNull(NavSystem);

	FNavLocation SpawnLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(GetActorLocation(), SpawnAreaRadius, SpawnLocation))
	{
		auto CharacterData = UDataTableManager::Get(this)->GetTableCsv<FCharacterData>(CharacterId);
		if (CharacterData.IsValid())
		{
			ABaseCharacter* Character = ABaseCharacter::Spawn(this, CharacterData, SpawnLocation, GetActorRotation().Quaternion());
			SpawnedCharacters.Add(Character);

			if (SpawnParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnParticle, Character->GetActorLocation(), Character->GetActorRotation(), true);
			}
		}
	}
}

void ACharacterSpawner::RemoveDestroyedCharacters()
{
	ReturnIfFalse(SpawnedCharacters.Num() > 0);

	for (int i = SpawnedCharacters.Num() - 1; i >= 0; --i)
	{
		if (SpawnedCharacters[i].IsValid() == false)
		{
			SpawnedCharacters.RemoveAt(i);
		}
	}
}

void ACharacterSpawner::CacheSpawnerCsv()
{
	ReturnIfNull(UDataTableManager::Get(this));
	SpawnerDataCsv = UDataTableManager::Get(this)->GetTableCsv<FSpawnerData>(SpawnerId);
}

void ACharacterSpawner::StartPick()
{
	StaticMesh->bRenderCustomDepth = 1;
	StaticMesh->CustomDepthStencilValue = 2;
	StaticMesh->MarkRenderStateDirty();
}

void ACharacterSpawner::EndPick()
{
	StaticMesh->bRenderCustomDepth = 0;
	StaticMesh->CustomDepthStencilValue = 0;
	StaticMesh->MarkRenderStateDirty();
}


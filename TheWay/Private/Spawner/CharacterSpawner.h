// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PickableActorInterface.h"
#include "CharacterSpawner.generated.h"

UCLASS()
class ACharacterSpawner : public AActor, public IPickableActorInterface
{
	GENERATED_BODY()

public:
	static ACharacterSpawner* Spawn(AActor* Owner, TSharedPtr<const struct FSpawnerData> SpawnerData);

public:	
	// Sets default values for this actor's properties
	ACharacterSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsMaxLevel() const;
	void LevelUp();
	void SetLevel(int32 Level);
	void SpawnCharacter(int32 CharacterId);

	const TSharedPtr<const struct FSpawnerData>& GetSpawnerDataCsv() const { return SpawnerDataCsv; }
	int32 GetSpawnerLevel() const { return SpawnerLevel; }
	float GetSpawnInterval() const { return SpawnInterval; }
	float GetSpawnAreaRadius() const { return SpawnAreaRadius; }
	int32 GetMaxSpawnNum() const { return MaxSpawnNum; }
	const TArray<struct FProbabilityData>& GetSpawnProbabilityList() const { return SpawnProbabilityList; }

	void SetSpawnerID(TPair<int32, int32> SpawnerId, bool bUpdateProperties = true);

protected:
	virtual void UpdateSpawn(const float DeltaTime);
	virtual void SpawnCharacter();
	void RemoveDestroyedCharacters();
	void CacheSpawnerCsv();

	/*
	* IPickableActorInterface
	*/
public:
	virtual void StartPick() override;
	virtual void EndPick() override;

public:
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class UStaticMeshComponent* StaticMesh;

private:
	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = true))
	int32 SpawnerLevel;

	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = true))
	float SpawnInterval; // 초 단위

	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = true))
	float SpawnAreaRadius;

	UPROPERTY(VisibleAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = true))
	int32 SpawnedNum;

	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = true))
	int32 MaxSpawnNum;

	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = true))
	TArray<struct FProbabilityData> SpawnProbabilityList;
	
	UPROPERTY(EditDefaultsOnly, Category = Particles)
	class UParticleSystem* SpawnParticle;

	TArray<TWeakObjectPtr<class ABaseCharacter>> SpawnedCharacters;

	float SpawnTimer;

	// Spawner는 Id, Level로 키값 사용
	TPair<int32, int32> SpawnerId;
	TSharedPtr<const struct FSpawnerData> SpawnerDataCsv;
};

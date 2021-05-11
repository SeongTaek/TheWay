// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectileSpawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UProjectileSpawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UProjectileSpawnComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	void Spawn(UClass* Class, FVector Location, FVector ShootDirection, class APawn* Instigator);
	void SetOwnerLevel(int32 InLevel) { OwnerLevel = InLevel; }

private:
	void SpawnInternal(UClass* Class, FVector Location, FVector ShootDirection, class APawn* Instigator);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AProjectileBase> OverrideProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector LocationOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector ShootAngleOffset; // Degree 단위

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ShootDelay; // Second 단위

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ProjectileInitialSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 RequiredOwnerMinLevel = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 RequiredOwnerMaxLevel = 999999;

	int32 OwnerLevel;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGameInstance.h"
#include "Tickable.h"
#include "TheWayGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UTheWayGameInstance : public UBaseGameInstance, public FTickableGameObject
{
	GENERATED_BODY()
	
public:
	UTheWayGameInstance();
	
public:
	virtual void Init() override;
	virtual void StartGameInstance() override;
	
public:
	// FTickableGameObject
	virtual bool IsTickableWhenPaused() const override { return true; }
	virtual bool IsTickableInEditor() const override { return true; }
	virtual ETickableTickType GetTickableTickType() const override { return (HasAnyFlags(RF_ClassDefaultObject) ? ETickableTickType::Never : ETickableTickType::Always); }
	virtual bool IsTickable() const { return !HasAnyFlags(RF_ClassDefaultObject); }
	virtual TStatId GetStatId() const override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY()
	class UDataTableManager* DataTableManager;
};

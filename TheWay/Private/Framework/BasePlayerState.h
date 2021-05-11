// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BasePlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChanged);

/**
 * 
 */
UCLASS()
class ABasePlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void AcquireItem(class AItemBase* Item);

	bool TryConsumeSoul(int32 Num);
	void AbsorbSoul(int32 Num);

	int32 GetSoulAbsorptionCount() const { return SoulAbsorptionCount; }
	int32 GetTotalSoulAbsorptionCount() const { return TotalSoulAbsorptionCount; }

public:
	FOnPlayerStateChanged OnPlayerStateChanged;

protected:
	int32 SoulAbsorptionCount;
	int32 TotalSoulAbsorptionCount;
};

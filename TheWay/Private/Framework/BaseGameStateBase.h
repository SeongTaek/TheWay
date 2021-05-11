// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BaseGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class ABaseGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	class ABasePlayerState* GetLocalPlayerState() const;
};

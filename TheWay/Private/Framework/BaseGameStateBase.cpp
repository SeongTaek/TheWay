// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/BaseGameStateBase.h"
#include "Framework/BasePlayerState.h"
#include "Utility/CreataekUtility.h"

class ABasePlayerState* ABaseGameStateBase::GetLocalPlayerState() const
{
	ReturnNullptrIfFalse(PlayerArray.Num() > 0);

	return Cast<ABasePlayerState>(PlayerArray[0]);
}

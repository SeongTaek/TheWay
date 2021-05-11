// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/BasePlayerState.h"
#include "Utility/CreataekUtility.h"
#include "Item/ItemBase.h"
#include "Item/Soul/SoulItem.h"

void ABasePlayerState::AcquireItem(class AItemBase* Item)
{
	if (Item->IsA(ASoulItem::StaticClass()))
	{
		AbsorbSoul(1);
	}
}

bool ABasePlayerState::TryConsumeSoul(int32 Num)
{
	ReturnFalseIfFalse(Num <= SoulAbsorptionCount);

	SoulAbsorptionCount -= Num;
	OnPlayerStateChanged.Broadcast();

	return true;
}

void ABasePlayerState::AbsorbSoul(int32 Num)
{
	SoulAbsorptionCount += Num;
	TotalSoulAbsorptionCount += Num;
	OnPlayerStateChanged.Broadcast();
}

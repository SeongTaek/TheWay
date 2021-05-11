// Copyright Epic Games, Inc. All Rights Reserved.


#include "TheWayGameModeBase.h"

ATheWayGameModeBase::ATheWayGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	PrimaryActorTick.bTickEvenWhenPaused = true;
}

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

THEWAY_API DECLARE_LOG_CATEGORY_EXTERN(LogTheWay, Log, All);

#define ECC_FRIENDLY_OVERLAP ECC_GameTraceChannel1
#define ECC_NEUTRAL_OVERLAP ECC_GameTraceChannel2
#define ECC_HOSTILE_OVERLAP ECC_GameTraceChannel3
#define ECC_FRIENDLY_PAWN ECC_GameTraceChannel4
#define ECC_NEUTRAL_PAWN ECC_GameTraceChannel5
#define ECC_HOSTILE_PAWN ECC_GameTraceChannel6
#define ECC_FRIENDLY_COLLISION ECC_GameTraceChannel7
#define ECC_NEUTRAL_COLLISION ECC_GameTraceChannel8
#define ECC_HOSTILE_COLLISION ECC_GameTraceChannel9
#define ECC_PICK ECC_GameTraceChannel10

#define COLLISION_PROFILE_FRIENDLY_PROEJCTILE_COLLISION (TEXT("FriendlyProejctileCollision"))
#define COLLISION_PROFILE_NEUTRAL_PROEJCTILE_COLLISION (TEXT("NeutralProejctileCollision"))
#define COLLISION_PROFILE_HOSTILE_PROEJCTILE_COLLISION (TEXT("HostileProejctileCollision"))
#define COLLISION_PROFILE_FRIENDLY_MELEE_COLLISION (TEXT("FriendlyCollision"))
#define COLLISION_PROFILE_NEUTRAL_MELEE_COLLISION (TEXT("NeutralCollision"))
#define COLLISION_PROFILE_HOSTILE_MELEE_COLLISION (TEXT("HostileCollision"))
#define COLLISION_PROFILE_FRIENDLY_PAWN (TEXT("FriendlyPawn"))
#define COLLISION_PROFILE_NEUTRAL_PAWN (TEXT("NeutralPawn"))
#define COLLISION_PROFILE_HOSTILE_PAWN (TEXT("HostilePawn"))

UENUM()
enum class EWidgetOrder : uint8
{
	Screen,
	DefaultHUD,
	InGameHUD,
	NoticeMessage,
};
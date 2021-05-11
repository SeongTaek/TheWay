// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/CreataekUtility.h"
#include "EngineUtils.h"

AActor* UCreataekUtility::SpawnEffect(TSubclassOf<AActor> InClass, const FTransform& InTransform, AActor* Owner)
{
	if (Owner == nullptr)
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;
	SpawnParams.Instigator = Owner->GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* EffectActor = Owner->GetWorld()->SpawnActor<AActor>(InClass, InTransform, SpawnParams);

	return EffectActor;
}

FString UCreataekUtility::MakeClassPath(const FString& ResourcePath, const FString* BasePaeh /*= nullptr*/)
{
	if (ResourcePath.IsEmpty())
	{
		return TEXT("");
	}

	int SplitIndex = 0;
	ResourcePath.FindLastChar('/', SplitIndex);

	FString Directory = ResourcePath.Left(SplitIndex + 1);
	FString FileName = ResourcePath.Mid(SplitIndex + 1);


	FString ResultPath;

	if (BasePaeh)
	{
		ResultPath = FString::Printf(TEXT("/%s/%s/%s.%s_C"), **BasePaeh, *Directory, *FileName, *FileName);
	}
	else
	{
		ResultPath = FString::Printf(TEXT("/%s/%s.%s_C"), *Directory, *FileName, *FileName);
	}

	FPaths::RemoveDuplicateSlashes(ResultPath);
	return ResultPath;
}

AActor* UCreataekUtility::GetClosestActor(UWorld* WorldContext, TSubclassOf<AActor> ActorClass, AActor* MyActor, FName Tag, const float MaxDistance)
{
	FVector MyLocation = MyActor->GetActorLocation();
	AActor* ClosestActor = nullptr;
	float CurrentClosestDistance = MaxDistance;

	for (TActorIterator<AActor> It(WorldContext, ActorClass); It; ++It)
	{
		AActor* Actor = *It;

		if (Actor == false || Actor == MyActor || Actor->IsPendingKill() || Actor->ActorHasTag(Tag) == false)
		{
			continue;
		}

		float distance = FVector::DistSquared(MyLocation, Actor->GetActorLocation());
		if (distance < CurrentClosestDistance)
		{
			CurrentClosestDistance = distance;
			ClosestActor = Actor;
		}
	}

	return ClosestActor;
}

AActor* UCreataekUtility::GetClosestActor(const TArray<FOverlapResult>& Overlaps, TSubclassOf<AActor> ActorClass, const FVector Location, const float MaxDistance)
{
	AActor* ClosestActor = nullptr;
	float CurrentClosestDistance = MaxDistance;

	for (const FOverlapResult& Result : Overlaps)
	{
		AActor* Actor = Result.GetActor();
		if (Actor == false || Actor->IsPendingKill() || Actor->IsA(ActorClass) == false)
		{
			continue;
		}

		float distance = FVector::DistSquared(Location, Actor->GetActorLocation());
		if (distance < CurrentClosestDistance)
		{
			CurrentClosestDistance = distance;
			ClosestActor = Actor;
		}
	}

	return ClosestActor;
}

ECollisionChannel UCreataekUtility::GetECCCollisionByTeamId(const FGenericTeamId& TeamId)
{
	switch (TeamId)
	{
	case ETeamAttitude::Friendly:
		return ECC_FRIENDLY_COLLISION;
	case ETeamAttitude::Hostile:
		return ECC_HOSTILE_COLLISION;
	case ETeamAttitude::Neutral:
	default:
		return ECC_NEUTRAL_COLLISION;
	}
}

ECollisionChannel UCreataekUtility::GetECCOverlapByTeamId(const FGenericTeamId& TeamId)
{
	switch (TeamId)
	{
	case ETeamAttitude::Friendly:
		return ECC_FRIENDLY_OVERLAP;
	case ETeamAttitude::Hostile:
		return ECC_HOSTILE_OVERLAP;
	case ETeamAttitude::Neutral:
	default:
		return ECC_NEUTRAL_OVERLAP;
	}
}

ECollisionChannel UCreataekUtility::GetECCPawnByTeamId(const FGenericTeamId& TeamId)
{
	switch (TeamId)
	{
	case ETeamAttitude::Friendly:
		return ECC_FRIENDLY_PAWN;
	case ETeamAttitude::Hostile:
		return ECC_HOSTILE_PAWN;
	case ETeamAttitude::Neutral:
	default:
		return ECC_NEUTRAL_PAWN;
	}
}

FName UCreataekUtility::GetProjectileCollisionProfileByTeamId(const FGenericTeamId& TeamId)
{
	switch (TeamId)
	{
	case ETeamAttitude::Friendly:
		return COLLISION_PROFILE_FRIENDLY_PROEJCTILE_COLLISION;
	case ETeamAttitude::Hostile:
		return COLLISION_PROFILE_HOSTILE_PROEJCTILE_COLLISION;
	case ETeamAttitude::Neutral:
	default:
		return COLLISION_PROFILE_NEUTRAL_PROEJCTILE_COLLISION;
	}
}

FName UCreataekUtility::GetOverlapCollisionProfileByTeamId(const FGenericTeamId& TeamId)
{
	switch (TeamId)
	{
	case ETeamAttitude::Friendly:
		return COLLISION_PROFILE_FRIENDLY_MELEE_COLLISION;
	case ETeamAttitude::Hostile:
		return COLLISION_PROFILE_HOSTILE_MELEE_COLLISION;
	case ETeamAttitude::Neutral:
	default:
		return COLLISION_PROFILE_NEUTRAL_MELEE_COLLISION;
	}
}


FName UCreataekUtility::GetPawnCollisionProfileByTeamId(const FGenericTeamId& TeamId)
{
	switch (TeamId)
	{
	case ETeamAttitude::Friendly:
		return COLLISION_PROFILE_FRIENDLY_PAWN;
	case ETeamAttitude::Hostile:
		return COLLISION_PROFILE_HOSTILE_PAWN;
	case ETeamAttitude::Neutral:
	default:
		return COLLISION_PROFILE_NEUTRAL_PAWN;
	}
}

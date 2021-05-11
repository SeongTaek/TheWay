// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheWay.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "CreataekUtility.generated.h"

#define ReturnIfNull(Value)			{ if ((Value) == nullptr) return; }
#define ReturnTrueIfNull(Value)			{ if ((Value) == nullptr) return true; }
#define ReturnFalseIfNull(Value)			{ if ((Value) == nullptr) return false; }
#define ReturnTrueIfTrue(Value)			{ if ((Value) == true) return true; }
#define ReturnTrueIfFalse(Value)			{ if ((Value) == false) return true; }
#define ReturnFalseIfTrue(Value)			{ if ((Value) == true) return false; }
#define ReturnFalseIfFalse(Value)			{ if ((Value) == false) return false; }
#define ReturnNullptrIfTrue(Value)			{ if ((Value) == true) return nullptr; }
#define ReturnNullptrIfFalse(Value)			{ if ((Value) == false) return nullptr; }
#define ReturnIfFalse(Statement)			{ if ((Statement) == false) return; }
#define ReturnIfTrue(Statement)			{ if ((Statement) == true) return; }

template<typename T, typename U>
T* CastStruct(U* base)
{
	static_assert(TIsDerivedFrom<T, U>::IsDerived, "Type T has to be derived from U.");
	return static_cast<T*>(base);
}

/**
 *
 */
UCLASS()
class UCreataekUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static AActor* SpawnEffect(TSubclassOf<class AActor> InClass, const FTransform& InTransform, AActor* Owner);

	static FString MakeClassPath(const FString& ResourcePath, const FString* BasePaeh = nullptr);

	static AActor* GetClosestActor(UWorld* WorldContext, TSubclassOf<AActor> ActorClass, AActor* MyActor, FName Tag, const float MaxDistance = TNumericLimits<float>::Max());
	static AActor* GetClosestActor(const TArray<FOverlapResult>& Overlaps, TSubclassOf<AActor> ActorClass, const FVector Location, const float MaxDistance);

	static ECollisionChannel GetECCCollisionByTeamId(const FGenericTeamId& TeamId);
	static ECollisionChannel GetECCOverlapByTeamId(const FGenericTeamId& TeamId);
	static ECollisionChannel GetECCPawnByTeamId(const FGenericTeamId& TeamId);

	static FName GetProjectileCollisionProfileByTeamId(const FGenericTeamId& TeamId);
	static FName GetOverlapCollisionProfileByTeamId(const FGenericTeamId& TeamId);
	static FName GetPawnCollisionProfileByTeamId(const FGenericTeamId& TeamId);

	static const FText& NewLine() { static FText _ = NSLOCTEXT("MiscUtil", "NewLine", "\n"); return _; }

	template<class LambdaType>
	static FTimerHandle DelayExecute(UWorld* InWorld, const LambdaType& InLambda, float InDelay = 0.0f)
	{
		check(InWorld);

		FTimerHandle TimerHandle;

		if (InDelay > 0.0f)
		{
			InWorld->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(InLambda), InDelay, false);
		}
		else
		{
			InWorld->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda(InLambda));
		}

		return TimerHandle;
	}
};

// UCreataekUtility 약어 / 편의점 CU 아님! ㅋㅋ
typedef UCreataekUtility CU;
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "TheWayAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "BehaviorTree Stop"))
class UTheWayAnimNotifyState_BehaviorTreeStop : public UAnimNotifyState
{
	GENERATED_BODY()
	
private:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

UCLASS(meta = (DisplayName = "Shield"))
class UTheWayAnimNotifyState_Shield : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

UCLASS(meta = (DisplayName = "SpringArm Movement"))
class UTheWayAnimNotifyState_SpringArmMovement : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float InTotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	UPROPERTY(EditAnywhere)
	float TargetArmLengthOffset;

	UPROPERTY(EditAnywhere)
	FVector LocationOffset;

	UPROPERTY(EditAnywhere)
	FRotator RotationOffset;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.1))
	float InterpDuration = 1.0f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.1))
	float InterpToOriginDuration = 1.0f;

	class USpringArmComponent* SpringArmComponent;

	float OriginTargetArmLength;
	FVector OriginLocation;
	FRotator OriginRotation;
	bool bOriginDoCollisionTest;

	float DestTargetArmLength;
	FVector DestLocation;
	FRotator DestRotation;

	float TotalDuration;
	float CurrentDuration;
	float HoldingDuration;
	float InterpDeltaTime;
	float InterpToOriginDeltaTime;
};
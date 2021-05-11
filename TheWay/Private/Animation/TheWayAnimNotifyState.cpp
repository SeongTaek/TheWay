// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/TheWayAnimNotifyState.h"
#include "Character/BaseCharacter.h"
#include "Controller/BaseAIController.h"
#include "GameFramework/SpringArmComponent.h"

void UTheWayAnimNotifyState_BehaviorTreeStop::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner()))
	{
		if (ABaseAIController* AIController = Cast<ABaseAIController>(Character->GetController()))
		{
			AIController->StopAI();
		}
	}
}

void UTheWayAnimNotifyState_BehaviorTreeStop::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner()))
	{
		if (ABaseAIController* AIController = Cast<ABaseAIController>(Character->GetController()))
		{
			AIController->RestartAI();
		}
	}
}

void UTheWayAnimNotifyState_Shield::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (MeshComp && MeshComp->GetOwner())
	{
		MeshComp->GetOwner()->SetActorEnableCollision(false);
	}
}

void UTheWayAnimNotifyState_Shield::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (MeshComp && MeshComp->GetOwner())
	{
		MeshComp->GetOwner()->SetActorEnableCollision(true);
	}
}

void UTheWayAnimNotifyState_SpringArmMovement::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float InTotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, InTotalDuration);

	CurrentDuration = 0.0f;
	InterpDeltaTime = 0.0f;
	InterpToOriginDeltaTime = 0.0f;
	TotalDuration = InTotalDuration;
	HoldingDuration = InTotalDuration - InterpToOriginDuration;

	if (MeshComp->GetOwner())
	{
		SpringArmComponent = Cast<USpringArmComponent>(MeshComp->GetOwner()->GetComponentByClass(USpringArmComponent::StaticClass()));
		if (SpringArmComponent)
		{
			OriginTargetArmLength = SpringArmComponent->TargetArmLength;
			OriginLocation = SpringArmComponent->GetRelativeLocation();
			OriginRotation = SpringArmComponent->GetRelativeRotation();
			bOriginDoCollisionTest = SpringArmComponent->bDoCollisionTest;
			DestTargetArmLength = OriginTargetArmLength + TargetArmLengthOffset;
			DestLocation = OriginLocation + LocationOffset;
			DestRotation = OriginRotation + RotationOffset;
			SpringArmComponent->bDoCollisionTest = false;
		}
	}
}

void UTheWayAnimNotifyState_SpringArmMovement::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	CurrentDuration += FrameDeltaTime;

	if (CurrentDuration < HoldingDuration)
	{
		if (SpringArmComponent)
		{
			InterpDeltaTime += FrameDeltaTime / InterpDuration;
			SpringArmComponent->TargetArmLength = FMath::FInterpTo(OriginTargetArmLength, DestTargetArmLength, InterpDeltaTime, 1.0f);
			SpringArmComponent->SetRelativeLocation(FMath::VInterpTo(OriginLocation, DestLocation, InterpDeltaTime, 1.0f));
			SpringArmComponent->SetRelativeRotation(FMath::RInterpTo(OriginRotation, DestRotation, InterpDeltaTime, 1.0f));
		}
	}
	else
	{
		if (SpringArmComponent)
		{
			InterpToOriginDeltaTime += FrameDeltaTime / InterpToOriginDuration;
			SpringArmComponent->TargetArmLength = FMath::FInterpTo(DestTargetArmLength, OriginTargetArmLength, InterpToOriginDeltaTime, 1.0f);
			SpringArmComponent->SetRelativeLocation(FMath::VInterpTo(DestLocation, OriginLocation, InterpToOriginDeltaTime, 1.0f));
			SpringArmComponent->SetRelativeRotation(FMath::RInterpTo(DestRotation, OriginRotation, InterpToOriginDeltaTime, 1.0f));
		}
	}
}

void UTheWayAnimNotifyState_SpringArmMovement::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (SpringArmComponent)
	{
		SpringArmComponent->TargetArmLength = OriginTargetArmLength;
		SpringArmComponent->SetRelativeLocation(OriginLocation);
		SpringArmComponent->SetRelativeRotation(OriginRotation);
		SpringArmComponent->bDoCollisionTest = bOriginDoCollisionTest;
	}
}

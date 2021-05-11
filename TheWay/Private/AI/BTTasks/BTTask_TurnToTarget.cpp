// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/BTTask_TurnToTarget.h"
#include "Character/BaseCharacter.h"
#include "Controller/BaseAIController.h"
#include "Utility/CreataekUtility.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

FString UBTTask_TurnToTarget::GetStaticDescription() const
{
	return FString::Printf(TEXT("목표물(%s)을 향해 회전합니다."), *BBKeyTarget.SelectedKeyName.ToString());
}

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("목표물 바라보기");

	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Character = Cast<ABaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	auto Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKeyTarget.SelectedKeyName));
	if (Target == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;

	return EBTNodeResult::InProgress;
}

void UBTTask_TurnToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	auto Character = Cast<ABaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	ReturnIfNull(Character);
	auto Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKeyTarget.SelectedKeyName));
	ReturnIfNull(Target);

	FVector Direction = Target->GetActorLocation() - Character->GetActorLocation();
	Direction.Normalize();

	FRotator CharacterRotator = Character->GetActorRotation();
	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;

	Character->SetActorRotation(FMath::RInterpTo(CharacterRotator, NewLookAt, DeltaSeconds, 10.0f));

	if (FMath::Abs(CharacterRotator.Yaw - NewLookAt.Yaw) <= 1.0f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_TurnToTarget::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	auto Character = Cast<ABaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	ReturnIfNull(Character);
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

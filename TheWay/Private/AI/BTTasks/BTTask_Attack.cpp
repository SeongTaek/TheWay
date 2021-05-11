// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/BTTask_Attack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utility/CreataekUtility.h"
#include "Character/BaseCharacter.h"
#include "Controller/BaseAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

FString UBTTask_Attack::GetStaticDescription() const
{
	if (bTurnToTarget)
	{
		return FString::Printf(TEXT("대상(%s)을 바라보며 공격합니다."), *BBKeyTarget.SelectedKeyName.ToString());
	}
	else
	{
		return FString::Printf(TEXT("대상(%s)을 공격합니다."), *BBKeyTarget.SelectedKeyName.ToString());
	}
}

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("공격하기");
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ABaseCharacter* Character = Cast<ABaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	Character->Attack();
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ABaseCharacter* Character = Cast<ABaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	else
	{
		if (Character->GetIsAttacking() == false)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
		else
		{
			if (bTurnToTarget)
			{
				auto Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKeyTarget.SelectedKeyName));
				ReturnIfNull(Target);

				FVector Direction = Target->GetActorLocation() - Character->GetActorLocation();
				Direction.Normalize();

				FRotator CharacterRotator = Character->GetActorRotation();
				FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
				NewLookAt.Pitch = 0.0f;
				NewLookAt.Roll = 0.0f;

				Character->SetActorRotation(FMath::RInterpTo(CharacterRotator, NewLookAt, DeltaSeconds, TurnSpeed));
			}
		}
	}
}

void UBTTask_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	auto Character = Cast<ABaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	ReturnIfNull(Character);
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServices/UBTService_TurnToTarget.h"
#include "Character/BaseCharacter.h"
#include "Controller/BaseAIController.h"
#include "Utility/CreataekUtility.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

FString UUBTService_TurnToTarget::GetStaticDescription() const
{
	return FString::Printf(TEXT("목표물(%s)을 향해 회전합니다."), *BBKeyTarget.SelectedKeyName.ToString());
}

UUBTService_TurnToTarget::UUBTService_TurnToTarget()
{
	NodeName = TEXT("목표물 바라보기");
}

void UUBTService_TurnToTarget::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	auto Character = Cast<ABaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)
	{
		return;
	}

	auto Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKeyTarget.SelectedKeyName));
	if (Target == nullptr)
	{
		return;
	}

	Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

void UUBTService_TurnToTarget::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);

	auto Character = Cast<ABaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	ReturnIfNull(Character);
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void UUBTService_TurnToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

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
}

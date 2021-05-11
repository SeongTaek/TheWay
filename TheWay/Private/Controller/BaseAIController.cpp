// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BaseAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Character/BaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Item/Weapon/WeaponBase.h"

ABaseAIController::ABaseAIController()
{

}

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(InPawn))
	{
		BaseCharacter->OnEquippedWeaponChanged.AddUObject(this, &ABaseAIController::OnEquippedWeaponChanged);
	}
	/*if (BaseCharacter)
	{
		BaseCharacter->OnChangeEquippedWeapon.AddLambda([this](AWeaponBase* LastWeapon, AWeaponBase* EquippedWeapon)
		{
			if (auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent))
			{
				BehaviorTreeComponent->GetBlackboardComponent()->SetValueAsFloat(AttackRangeKey, EquippedWeapon->GetWeaponStatComponent()->GetAttackRange());
			}
		});
	}*/
}

void ABaseAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ABaseAIController::RestartAI()
{
	if (auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent))
	{
		BehaviorTreeComponent->RestartTree();
	}
}

void ABaseAIController::StopAI()
{
	if (auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent))
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}

void ABaseAIController::OnEquippedWeaponChanged(AWeaponBase* LastWeapon, AWeaponBase* EquippedWeapon)
{
	NotifyEquippedWeaponChanged.Broadcast(this);

	if (OnEquippedWeaponLevelUpHandle.IsValid())
	{
		OnEquippedWeaponLevelUpHandle.Reset();
	}

	OnEquippedWeaponLevelUpHandle = EquippedWeapon->OnWeaponLevelUp.AddUObject(this, &ABaseAIController::OnEquippedWeaponLevelUp);
}

void ABaseAIController::OnEquippedWeaponLevelUp()
{
	NotifyEquippedWeaponLevelUp.Broadcast(this);
}


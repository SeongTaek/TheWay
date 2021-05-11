// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServices/BTService_CheckObstacleBetween.h"
#include "Character/BaseCharacter.h"
#include "Controller/BaseAIController.h"
#include "Utility/CreataekUtility.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Item/Weapon/WeaponBase.h"
#include "Item/Weapon/RangedWeapon/RangedWeaponBase.h"
#include "Item/Weapon/MeleeWeapon/MeleeWeaponBase.h"

FString UBTService_CheckObstacleBetween::GetStaticDescription() const
{
	return FString::Printf(TEXT("대상(%s) 사이에 장애물이 없으면 Task를 성공시킵니다."), *BBKeyTarget.SelectedKeyName.ToString());
}

UBTService_CheckObstacleBetween::UBTService_CheckObstacleBetween()
{
	NodeName = TEXT("대상 사이에 장애물 확인");
}

void UBTService_CheckObstacleBetween::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	ReturnIfFalse(ControllingPawn);

	ABaseCharacter* ControllingCharacter = Cast<ABaseCharacter>(ControllingPawn);
	ReturnIfFalse(ControllingCharacter);

	auto Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKeyTarget.SelectedKeyName));
	ReturnIfFalse(Target);

	// 타겟 사이에 장애물이 있는지 체크
	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	ECollisionChannel TraceChannel = ControllingCharacter->GetProjectileCollisionChannel();

	GetWorld()->LineTraceSingleByChannel(Hit, ControllingCharacter->GetActorLocation(), Target->GetActorLocation(), TraceChannel, CollisionParams);

	if (Hit.GetActor() == nullptr || Hit.GetActor() == Target || Hit.GetActor()->GetParentActor() == Target || Cast<ABaseCharacter>(Hit.GetActor()) != nullptr)
	{
		OwnerComp.RequestExecution(EBTNodeResult::Succeeded);
	}
}

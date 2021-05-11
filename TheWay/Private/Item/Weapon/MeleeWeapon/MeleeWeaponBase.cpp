// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapon/MeleeWeapon/MeleeWeaponBase.h"
#include "Components/WeaponStatComponent.h"
#include "Character/BaseCharacter.h"
#include "Utility/CreataekUtility.h"
#include "TheWay.h"

AMeleeWeaponBase::AMeleeWeaponBase()
{
	AttackCheckCount = 1;
	AttackCheckRate = 0.1f;
	AttackCheckFirstDelay = 0.0f;
	CurrentAttackCheckCount = 0;
}

void AMeleeWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AMeleeWeaponBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GetComponents<UPrimitiveComponent>(AttackCheckCollisions);
}

void AMeleeWeaponBase::StartAttack()
{
}

void AMeleeWeaponBase::DecideAttack()
{
	CurrentAttackCheckCount = 0;
	GetWorldTimerManager().ClearTimer(AttackCheckTimer);
	GetWorldTimerManager().SetTimer(AttackCheckTimer, this, &AMeleeWeaponBase::AttackCheckCollision, AttackCheckRate, true, AttackCheckFirstDelay);
}

void AMeleeWeaponBase::OnEquip(class ABaseCharacter* InOwnerCharacter)
{
	Super::OnEquip(InOwnerCharacter);

	SetGenericTeamId(InOwnerCharacter->GetGenericTeamId());
}

void AMeleeWeaponBase::AttackCheckCollision()
{
	for (auto Collision : AttackCheckCollisions)
	{
		const TArray<FOverlapInfo>& OverlapInfos = Collision->GetOverlapInfos();

		for (const FOverlapInfo& OverlapInfo : OverlapInfos)
		{
			if (ABaseCharacter* Character = Cast<ABaseCharacter>(OverlapInfo.OverlapInfo.Actor))
			{
				FPointDamageEvent DamageEvent;
				DamageEvent.Damage = WeaponStatComponent->GetAttackDamage();
				DamageEvent.HitInfo = OverlapInfo.OverlapInfo;
				DamageEvent.ShotDirection = (OverlapInfo.OverlapInfo.Actor->GetActorLocation() - OwnerCharacter->GetActorLocation()).GetSafeNormal2D();

				Character->TakeDamage(GetFinalAttackDamage(), DamageEvent, OwnerCharacter->Controller, this);
				Character->LaunchCharacter(DamageEvent.ShotDirection * 1000.0f, true, false);
			}
		}
	}

	if (++CurrentAttackCheckCount >= AttackCheckCount)
	{
		CurrentAttackCheckCount = 0;
		GetWorldTimerManager().ClearTimer(AttackCheckTimer);
	}
}

void AMeleeWeaponBase::SetGenericTeamId(const FGenericTeamId& NewTeamId)
{
	Super::SetGenericTeamId(NewTeamId);

	for (auto Collision : AttackCheckCollisions)
	{
		Collision->SetCollisionProfileName(CU::GetOverlapCollisionProfileByTeamId(NewTeamId));
	}
}

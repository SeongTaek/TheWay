// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAnimInstance.h"
#include "Utility/CreataekUtility.h"
#include "Controller/BaseAIController.h"
#include "CollisionObject/CollisionObject.h"
#include "Character/BaseCharacter.h"
#include "Item/Weapon/MeleeWeapon/MeleeWeaponBase.h"
#include "Item/Weapon/RangedWeapon/RangedWeaponBase.h"

UBaseAnimInstance::UBaseAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	CurrentPawnRotation = 0.0f;
	bDead = false;
}

void UBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OnMontageStarted.AddDynamic(this, &UBaseAnimInstance::HandleOnMontageStarted);
	OnMontageEnded.AddDynamic(this, &UBaseAnimInstance::HandleOnMontageEnded);
}

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();

	ReturnIfNull(Pawn);

	if (bDead == false)
	{
		// AI ��Ʈ�ѷ��� ������ �� BTTask_MoveTo�� �̵��ϴµ� ��ǥ ������ �����ϸ� Velocity ���� ��� 0���� �����ȴ�.
		// Velocity(CurrentPawnSpeed) ������ Idle, Walk �ִϸ��̼� ���� �ϴµ� ��� 0���� �����Ǹ� ����鼭 Idle �ִϸ��̼����� �Ѿ��
		// �ӽ÷� Velocity �� �����ؼ� CurrentPawnSpeed �����Ѵ�. (���� ���� �ʿ�!!)
		CurrentPawnSpeed = Pawn->GetVelocity().Size();// FMath::Lerp(CurrentPawnSpeed, Pawn->GetVelocity().Size(), DeltaSeconds * 20.0f);
		CurrentPawnRotation = CalculateDirection(Pawn->GetVelocity(), Pawn->GetActorRotation());

		//UE_LOG(LogTemp, Warning, TEXT("Speed : %f"), CurrentPawnSpeed);
		//UE_LOG(LogTemp, Warning, TEXT("Rotation : %f"), CurrentPawnRotation);
	}
}

FAnimMontagePlayInfo UBaseAnimInstance::ChooseIntroAnimMontage()
{
	IntroMontagePlayInfo = BP_ChooseIntroAnimMontage();
	return IntroMontagePlayInfo;
}

FAnimMontagePlayInfo UBaseAnimInstance::ChooseAttackAnimMontage()
{
	AttackMontagePlayInfo = BP_ChooseAttackAnimMontage();
	return AttackMontagePlayInfo;
}

FAnimMontagePlayInfo UBaseAnimInstance::ChooseSkillAnimMontage(int32 SkillIndex)
{
	SkillMontagePlayInfo = BP_ChooseSkillAnimMontage(SkillIndex);
	return SkillMontagePlayInfo;
}

FAnimMontagePlayInfo UBaseAnimInstance::ChooseDeathAnimMontage()
{
	bDead = true;
	DeathMontagePlayInfo = BP_ChooseDeathAnimMontage();
	return DeathMontagePlayInfo;
}

FAnimMontagePlayInfo UBaseAnimInstance::ChooseDamagedAnimMontage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageEvent.IsOfType(FImpulseDamageEvent::ClassID))
	{
		const auto ImpulseDamageEvent = (FImpulseDamageEvent*)(&DamageEvent);
		//DamagedMontagePlayInfo = BP_ChooseDamagedAnimMontage(NAME_None, ImpulseDamageEvent->ShotDirection);
		DamagedMontagePlayInfo = BP_ChooseDamagedAnimMontage(true);
		return DamagedMontagePlayInfo;
	}
	else if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const auto PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);
		//DamagedMontagePlayInfo = BP_ChooseDamagedAnimMontage(NAME_None, PointDamageEvent->ShotDirection);
		DamagedMontagePlayInfo = BP_ChooseDamagedAnimMontage(false);
		return DamagedMontagePlayInfo;
	}

	return FAnimMontagePlayInfo();
}

void UBaseAnimInstance::HandleOnMontageStarted(UAnimMontage* Montage)
{
	if (Montage == IntroMontagePlayInfo.MontageToPlay
		|| Montage == SkillMontagePlayInfo.MontageToPlay)
	{
		bPlayOnlyMontage = true;
	}
}

void UBaseAnimInstance::HandleOnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == IntroMontagePlayInfo.MontageToPlay
		|| Montage == SkillMontagePlayInfo.MontageToPlay)
	{
		bPlayOnlyMontage = false;
	}
}

bool UBaseAnimInstance::IsMeleeWeapon() const
{
	ABaseCharacter* Character = Cast<ABaseCharacter>(TryGetPawnOwner());
	ReturnFalseIfNull(Character);

	return Cast<AMeleeWeaponBase>(Character->GetEquippedWeapon()) != nullptr;
}

bool UBaseAnimInstance::IsRangedWeapon() const
{
	ABaseCharacter* Character = Cast<ABaseCharacter>(TryGetPawnOwner());
	ReturnFalseIfNull(Character);

	return Cast<ARangedWeaponBase>(Character->GetEquippedWeapon()) != nullptr;
}

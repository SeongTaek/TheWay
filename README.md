# TheWay
- 엔진 : Unreal Engine 4.26.2

# 플레이 영상

![image](https://user-images.githubusercontent.com/4263119/117795339-0e976a80-b289-11eb-9b03-7017a4705566.png)

# 기술서
## 캐릭터
### [ABaseCharacter](https://github.com/SeongTaek/TheWay/blob/main/TheWay/Private/Character/BaseCharacter.h)
* UBaseAnimInstance - 애니메이션 컨트롤
* UCharacterStatComponent - 캐릭터 스탯 관리
* AWeaponBase - 무기 컨트롤
* FCharacterData - 캐릭터 스탯, 무기 정보

> 공격 처리
```c++
ReturnIfFalse(CanAttack());
// 장착한 무기로 공격을 시작한다.
EquippedWeapon->StartAttack();

if (IsAttacking)
{
	if (FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo) && CanNextCombo)
	{
		IsComboInputOn = true;
	}
}
else
{
	if (CurrentCombo == 0)
	{
		// 공격 애니메이션을 불러온다.
		FAnimMontagePlayInfo MontagePlayInfo = AnimInstance->ChooseAttackAnimMontage();
		if (MontagePlayInfo.MontageToPlay)
		{
			PlayAnimMontage(MontagePlayInfo.MontageToPlay, MontagePlayInfo.PlayRate, MontagePlayInfo.Section);
		}

		AttackStartComboState();
		IsAttacking = true;
	}
}
```
> 피격 처리
``` c++
float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

if (IsDead())
{
	return FinalDamage;
}

if (DamagedSound)
{
	UGameplayStatics::PlaySoundAtLocation(this, DamagedSound, GetActorLocation(), GetActorRotation());
}

if (EventInstigator)
{
	SetInstigator(EventInstigator->GetPawn());
}
// HP 감소
CharacterStatComponent->TakeDamage(FinalDamage);
UpdateDamagedAnimation(FinalDamage, DamageEvent, EventInstigator, DamageCauser);
BP_SetMaterialEffect(ECharacterMaterialEffect::DAMAGED);

return FinalDamage;
```
### [UBaseAnimInstance](https://github.com/SeongTaek/TheWay/blob/main/TheWay/Private/Animation/BaseAnimInstance.h)
* FAnimMontagePlayInfo - 몽타주 정보

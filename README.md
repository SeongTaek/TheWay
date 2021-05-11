# TheWay
- 엔진 : Unreal Engine 4.26.2

# 플레이 영상

![image](https://user-images.githubusercontent.com/4263119/117795339-0e976a80-b289-11eb-9b03-7017a4705566.png)

# 캐릭터
### [ABaseCharacter](https://github.com/SeongTaek/TheWay/blob/main/TheWay/Private/Character/BaseCharacter.h)
* [UBaseAnimInstance](https://github.com/SeongTaek/TheWay/blob/main/TheWay/Private/Animation/BaseAnimInstance.h) - 애니메이션 컨트롤
* [UCharacterStatComponent](https://github.com/SeongTaek/TheWay/blob/main/TheWay/Private/Components/CharacterStatComponent.h) - 캐릭터 스탯 관리
* [AWeaponBase](https://github.com/SeongTaek/TheWay/blob/main/TheWay/Private/Item/Weapon/WeaponBase.h) - 무기 컨트롤
* [FCharacterData](https://github.com/SeongTaek/TheWay/blob/main/TheWay/Private/DataTable/TheWayDataTable.h) - 캐릭터 스탯, 무기 정보

> 공격 처리
```c++
void ABaseCharacter::Attack()
{
	ReturnIfFalse(CanAttack());
	// 무기로 공격하기
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
			// 공격 애니메이션 재생
			FAnimMontagePlayInfo MontagePlayInfo = AnimInstance->ChooseAttackAnimMontage();
			if (MontagePlayInfo.MontageToPlay)
			{
				PlayAnimMontage(MontagePlayInfo.MontageToPlay, MontagePlayInfo.PlayRate, MontagePlayInfo.Section);
			}

			AttackStartComboState();
			IsAttacking = true;
		}
	}
}
```
> 피격 처리
``` c++
float ABaseCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
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

	// 스탯 컴포넌트에서 HP 감소
	CharacterStatComponent->TakeDamage(FinalDamage);
	// 피격 애니메이션 재생
	UpdateDamagedAnimation(FinalDamage, DamageEvent, EventInstigator, DamageCauser);
	BP_SetMaterialEffect(ECharacterMaterialEffect::DAMAGED);

	return FinalDamage;
}
```
> 죽음 처리
> 
``` c++
void ABaseCharacter::PostInitializeComponents()
{
	...
	CharacterStatComponent->OnHPIsZero.AddUObject(this, &ABaseCharacter::OnDeath);
}
void ABaseCharacter::OnDeath()
{
	...
	FAnimMontagePlayInfo MontagePlayInfo = AnimInstance->ChooseDeathAnimMontage();
	if (MontagePlayInfo.MontageToPlay)
	{
		PlayAnimMontage(MontagePlayInfo.MontageToPlay, MontagePlayInfo.PlayRate, MontagePlayInfo.Section);

		if (bLifeSpan)
		{
			SetLifeSpan(MontagePlayInfo.MontageToPlay->GetPlayLength());
		}
	}
	else
	{
		if (bLifeSpan)
		{
			SetLifeSpan(LifeSpan);
		}
	}

	DropItems();

	if (OnCharacterDeath.IsBound())
	{
		OnCharacterDeath.Broadcast();
	}
}
```
# 애니메이션
* [UBaseAnimInstance](https://github.com/SeongTaek/TheWay/blob/main/TheWay/Private/Animation/BaseAnimInstance.h)
* [AnimNotify](https://github.com/SeongTaek/TheWay/blob/main/TheWay/Private/Animation/TheWayAnimNotify.h)
* [AnimNotifyState](https://github.com/SeongTaek/TheWay/blob/main/TheWay/Private/Animation/TheWayAnimNotifyState.h)
> 애님 그래프
![image](https://user-images.githubusercontent.com/4263119/117832508-81680c00-b2b0-11eb-844c-bfcea6892c11.png)
![image](https://user-images.githubusercontent.com/4263119/117830742-e3c00d00-b2ae-11eb-97f4-8e681ac5165c.png)
> 애님 노티파이 사용
![image](https://user-images.githubusercontent.com/4263119/117836030-516e3800-b2b3-11eb-8e03-ece65b01c358.png)

# AI
* [ABaseAIController](https://github.com/SeongTaek/TheWay/blob/main/TheWay/Private/Controller/BaseAIController.h)
* [BTTask](https://github.com/SeongTaek/TheWay/tree/main/TheWay/Private/AI/BTTasks)
* [BTDecorator](https://github.com/SeongTaek/TheWay/tree/main/TheWay/Private/AI/Decorators)
* [BTService](https://github.com/SeongTaek/TheWay/tree/main/TheWay/Private/AI/BTServices)
> Behavior Tree
![image](https://user-images.githubusercontent.com/4263119/117833489-57fbb000-b2b1-11eb-8d86-17be1264202a.png)

# UI
* [UActionSlotsWidget](https://github.com/SeongTaek/TheWay/blob/main/TheWay/Private/UI/ActionSlotsWidget.h)
* [USoulPointsLabelWidget](https://github.com/SeongTaek/TheWay/blob/main/TheWay/Private/UI/SoulPointsLabelWidget.h)
* [UHealthBarWidget](https://github.com/SeongTaek/TheWay/blob/main/TheWay/Private/UI/InGameHUDWidget.h)
> InGame
![image](https://user-images.githubusercontent.com/4263119/117839515-42d55000-b2b6-11eb-8e8f-42ac76144e82.png)


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"

USTRUCT(BlueprintType)
struct FAnimMontagePlayInfo
{
	GENERATED_BODY()

public:
	FAnimMontagePlayInfo() : MontageToPlay(nullptr), Section(NAME_None), PlayRate(1.0f) {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UAnimMontage* MontageToPlay;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, AdvancedDisplay)
	FName Section;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, AdvancedDisplay)
	float PlayRate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, AdvancedDisplay)
	bool IsFlying;
};

/**
 * 
 */
UCLASS()
class UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UBaseAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual FAnimMontagePlayInfo ChooseIntroAnimMontage();
	UFUNCTION(BlueprintImplementableEvent)
	FAnimMontagePlayInfo BP_ChooseIntroAnimMontage();

	virtual FAnimMontagePlayInfo ChooseAttackAnimMontage();
	UFUNCTION(BlueprintImplementableEvent)
	FAnimMontagePlayInfo BP_ChooseAttackAnimMontage();

	virtual FAnimMontagePlayInfo ChooseSkillAnimMontage(int32 SkillIndex);
	UFUNCTION(BlueprintImplementableEvent)
	FAnimMontagePlayInfo BP_ChooseSkillAnimMontage(int32 SkillIndex);

	virtual FAnimMontagePlayInfo ChooseDeathAnimMontage();
	UFUNCTION(BlueprintImplementableEvent)
	FAnimMontagePlayInfo BP_ChooseDeathAnimMontage();

	virtual FAnimMontagePlayInfo ChooseDamagedAnimMontage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	UFUNCTION(BlueprintImplementableEvent)
	FAnimMontagePlayInfo BP_ChooseDamagedAnimMontage(bool bWantDisableRootMotion/*FName DamagedBoneName, const FVector& DamagedLocation*/);
	
	const FAnimMontagePlayInfo& GetIntroMontagePlayInfo() const { return IntroMontagePlayInfo; }
	const FAnimMontagePlayInfo& GetAttackMontagePlayInfo() const { return AttackMontagePlayInfo; }
	const FAnimMontagePlayInfo& GetSkillMontagePlayInfo() const { return SkillMontagePlayInfo; }
	const FAnimMontagePlayInfo& GetDeathMontagePlayInfo() const { return DeathMontagePlayInfo; }
	const FAnimMontagePlayInfo& GetDamagedMontagePlayInfo() const { return DamagedMontagePlayInfo; }

	const float GetCurrentPawnSpeed() const { return CurrentPawnSpeed; }
	const float GetCurrentPawnRotation() const { return CurrentPawnRotation; }

protected:
	UFUNCTION()
	virtual void HandleOnMontageStarted(UAnimMontage* Montage);

	UFUNCTION()
	virtual void HandleOnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable)
	bool IsMeleeWeapon() const;

	UFUNCTION(BlueprintCallable)
	bool IsRangedWeapon() const;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bPlayOnlyMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montages, Meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*> IntroMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montages, Meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*> AttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montages, Meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*> SkillMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montages, Meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*> DeathMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montages, Meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*> DamagedMontages;

	UPROPERTY(VisibleAnywhere, Category = MontagePlayInfo, Meta = (AllowPrivateAccess = true))
	FAnimMontagePlayInfo IntroMontagePlayInfo;

	UPROPERTY(VisibleAnywhere, Category = MontagePlayInfo, Meta = (AllowPrivateAccess = true))
	FAnimMontagePlayInfo AttackMontagePlayInfo;

	UPROPERTY(VisibleAnywhere, Category = MontagePlayInfo, Meta = (AllowPrivateAccess = true))
	FAnimMontagePlayInfo SkillMontagePlayInfo;

	UPROPERTY(VisibleAnywhere, Category = MontagePlayInfo, Meta = (AllowPrivateAccess = true))
	FAnimMontagePlayInfo DeathMontagePlayInfo;

	UPROPERTY(VisibleAnywhere, Category = MontagePlayInfo, Meta = (AllowPrivateAccess = true))
	FAnimMontagePlayInfo DamagedMontagePlayInfo;
};

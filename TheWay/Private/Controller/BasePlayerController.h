// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPickActor, TWeakObjectPtr<AActor>);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnViewCharacterChanged, class ABaseCharacter*, class ABaseCharacter*);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnViewCharacterEquippedWeaponChanged, class AWeaponBase*, class AWeaponBase*);

/**
 * 
 */
UCLASS()
class ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ABasePlayerController();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void SetupInputComponent() override;

public:
	class AInGameHUD* GetInGameHUD() const;
	class UInGameHUDWidget* GetInGameHUDWidget() const;
	class ATheWayInGameState* GetInGameState() const;
	class ABasePlayerState* GetLocalPlayerState() const;
	const class ABaseCharacter* GetViewCharacter() const;

	void SetViewCharacater(class ABaseCharacter* TargetCharacter);

	void ConsumeHealthPotion();
	void LevelUpWeapon();
	bool CanLevelUpWeapon();
	void UseSkill(int Index);
	bool CanUseSkill() const;
	bool HasSkill(int Index);
	void Dash();
	bool CanDash() const;
	bool IsViewCharacterDead() const;

protected:
	UFUNCTION()
	virtual void ItemAcquired(const AItemBase* Item);

	UFUNCTION()
	virtual void OnViewCharacterDeath();

	UFUNCTION()
	virtual void OnEquippedWeaponChanged(class AWeaponBase* LastWeapon, class AWeaponBase* EquippedWeapon);

	void OnPick();

public:
	FOnPickActor OnPickActor;
	FOnViewCharacterChanged OnViewCharacterChanged;
	FOnViewCharacterEquippedWeaponChanged OnViewCharacterEquippedWeaponChanged;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sounds)
	class USoundBase* ConsumeHealthPotionSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sounds)
	class USoundBase* LevelUpWeaponSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sounds)
	class USoundBase* ItemAcquiredSound;

	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;

	UPROPERTY()
	TWeakObjectPtr<AActor> PickedActor;

	TWeakObjectPtr<class ABaseCharacter> ViewCharacter;
};

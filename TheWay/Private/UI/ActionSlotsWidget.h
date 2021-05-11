// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUserWidget.h"
#include "Components/Button.h"
#include "ActionSlotsWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInputButtonClickedEvent);
DECLARE_DELEGATE_RetVal(bool, FCanInputButtonClick);

/**
 * 
 */
UCLASS()
class THEWAY_API UActionSlotsWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnHealthPotionButtonClicked();

	UFUNCTION()
	void OnLevelUpWeaponClicked();

	UFUNCTION()
	void OnSkill1ButtonClicked();

	UFUNCTION()
	void OnSkill2ButtonClicked();

	UFUNCTION()
	void OnDashButtonClicked();

	UFUNCTION()
	void OnViewCharacterChanged(class ABaseCharacter* OldCharacter, class ABaseCharacter* NewCharacter);

	UFUNCTION()
	void OnEquippedWeaponChanged(class AWeaponBase* LastWeapon, class AWeaponBase* EquippedWeapon);

protected:
	UPROPERTY(meta = (BindWidget))
	class UInputButtonWidget* HealthPotionButton;

	UPROPERTY(meta = (BindWidget))
	class UInputButtonWidget* LevelUpWeaponButton;

	UPROPERTY(meta = (BindWidget))
	class UInputButtonWidget* SkillButton1;

	UPROPERTY(meta = (BindWidget))
	class UInputButtonWidget* SkillButton2;

	UPROPERTY(meta = (BindWidget))
	class UInputButtonWidget* DashButton;

	class ABasePlayerController* PlayerController;
};

UCLASS()
class THEWAY_API UInputButtonWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void InvokeButtonClick() { Btn_Click->OnClicked.Broadcast(); }

	int32 GetNeedValue() const { return NeedValue; }

	void SetNeedValue(int32 InNeedValue);

	bool CanClick();
	void LockButton();
	void UnlockButton();

protected:
	UFUNCTION()
	void OnButtonClicked();

public:
	FOnInputButtonClickedEvent OnClicked;
	FCanInputButtonClick ClickCondition;

protected:
	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "PaperSprite, Texture2D"))
	UObject* Resource;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Icon;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Ready;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Click;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_Cooldown;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_KeyLabel;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_NeedValue;

	UPROPERTY(EditAnywhere)
	FText KeyLabel;

	UPROPERTY(EditAnywhere)
	int32 NeedValue;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.1f))
	float Cooldown = 1.0f;

	float CooldownTimer;
};

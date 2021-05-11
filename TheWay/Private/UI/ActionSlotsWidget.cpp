// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ActionSlotsWidget.h"
#include "Utility/CreataekUtility.h"
#include "Controller/BasePlayerController.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Framework/BasePlayerState.h"
#include "Item/Weapon/WeaponBase.h"

void UActionSlotsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UActionSlotsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FInputActionBinding HealthPotionPressed("HealthPotion", IE_Pressed);
	FInputActionBinding LevelUpWeaponPressed("LevelUpWeapon", IE_Pressed);
	FInputActionBinding UseSkill1Pressed("UseSkill1", IE_Pressed);
	FInputActionBinding UseSkill2Pressed("UseSkill2", IE_Pressed);
	FInputActionBinding DashPressed("Dash", IE_Pressed);
	HealthPotionPressed.ActionDelegate.GetDelegateForManualSet().BindLambda([this]() { HealthPotionButton->InvokeButtonClick(); });
	LevelUpWeaponPressed.ActionDelegate.GetDelegateForManualSet().BindLambda([this]() { LevelUpWeaponButton->InvokeButtonClick(); });
	UseSkill1Pressed.ActionDelegate.GetDelegateForManualSet().BindLambda([this]() { SkillButton1->InvokeButtonClick(); });
	UseSkill2Pressed.ActionDelegate.GetDelegateForManualSet().BindLambda([this]() { SkillButton2->InvokeButtonClick(); });
	DashPressed.ActionDelegate.GetDelegateForManualSet().BindLambda([this]() { DashButton->InvokeButtonClick(); });

	PlayerController = GetPlayerController();
	PlayerController->InputComponent->AddActionBinding(HealthPotionPressed);
	PlayerController->InputComponent->AddActionBinding(LevelUpWeaponPressed);
	PlayerController->InputComponent->AddActionBinding(UseSkill1Pressed);
	PlayerController->InputComponent->AddActionBinding(UseSkill2Pressed);
	PlayerController->InputComponent->AddActionBinding(DashPressed);
	PlayerController->OnViewCharacterChanged.AddUObject(this, &UActionSlotsWidget::OnViewCharacterChanged);
	PlayerController->OnViewCharacterEquippedWeaponChanged.AddUObject(this, &UActionSlotsWidget::OnEquippedWeaponChanged);

	HealthPotionButton->OnClicked.AddDynamic(this, &UActionSlotsWidget::OnHealthPotionButtonClicked);
	LevelUpWeaponButton->OnClicked.AddDynamic(this, &UActionSlotsWidget::OnLevelUpWeaponClicked);
	SkillButton1->OnClicked.AddDynamic(this, &UActionSlotsWidget::OnSkill1ButtonClicked);
	SkillButton2->OnClicked.AddDynamic(this, &UActionSlotsWidget::OnSkill2ButtonClicked);
	DashButton->OnClicked.AddDynamic(this, &UActionSlotsWidget::OnDashButtonClicked);
	SkillButton1->ClickCondition.BindLambda([this]() -> bool { return GetPlayerController()->CanUseSkill(); });
	SkillButton2->ClickCondition.BindLambda([this]() -> bool { return GetPlayerController()->CanUseSkill(); });
	DashButton->ClickCondition.BindLambda([this]() -> bool { return GetPlayerController()->CanDash(); });
}

void UActionSlotsWidget::OnHealthPotionButtonClicked()
{
	if (PlayerController->IsViewCharacterDead() == false)
	{
		PlayerController->ConsumeHealthPotion();
		PlayerController->GetLocalPlayerState()->TryConsumeSoul(HealthPotionButton->GetNeedValue());
	}
}

void UActionSlotsWidget::OnLevelUpWeaponClicked()
{
	if (PlayerController->CanLevelUpWeapon())
	{
		PlayerController->LevelUpWeapon();
		PlayerController->GetLocalPlayerState()->TryConsumeSoul(LevelUpWeaponButton->GetNeedValue());

		if (PlayerController->CanLevelUpWeapon())
		{
			LevelUpWeaponButton->SetNeedValue(LevelUpWeaponButton->GetNeedValue() * 2);
		}
		else
		{
			LevelUpWeaponButton->SetNeedValue(0);
			LevelUpWeaponButton->LockButton();
		}
	}
}

void UActionSlotsWidget::OnSkill1ButtonClicked()
{
	if (PlayerController->CanUseSkill())
	{
		PlayerController->UseSkill(0);
		PlayerController->GetLocalPlayerState()->TryConsumeSoul(SkillButton1->GetNeedValue());
	}
}

void UActionSlotsWidget::OnSkill2ButtonClicked()
{
	if (PlayerController->CanUseSkill())
	{
		PlayerController->UseSkill(1);
		PlayerController->GetLocalPlayerState()->TryConsumeSoul(SkillButton2->GetNeedValue());
	}
}

void UActionSlotsWidget::OnDashButtonClicked()
{
	if (PlayerController->CanDash())
	{
		PlayerController->Dash();
		PlayerController->GetLocalPlayerState()->TryConsumeSoul(DashButton->GetNeedValue());
	}
}

void UActionSlotsWidget::OnViewCharacterChanged(ABaseCharacter* OldCharacter, ABaseCharacter* NewCharacter)
{
	if (PlayerController->CanLevelUpWeapon() == false)
	{
		LevelUpWeaponButton->SetNeedValue(0);
		LevelUpWeaponButton->LockButton();
	}
	else
	{
		LevelUpWeaponButton->SetNeedValue(2);
		LevelUpWeaponButton->UnlockButton();
	}

	if (PlayerController->HasSkill(0) == false)
	{
		SkillButton1->LockButton();
	}

	if (PlayerController->HasSkill(1) == false)
	{
		SkillButton2->LockButton();
	}
}

void UActionSlotsWidget::OnEquippedWeaponChanged(AWeaponBase* LastWeapon, AWeaponBase* EquippedWeapon)
{
	if (PlayerController->CanLevelUpWeapon() == false)
	{
		LevelUpWeaponButton->SetNeedValue(0);
		LevelUpWeaponButton->LockButton();
	}
	else
	{
		LevelUpWeaponButton->SetNeedValue(FMath::Pow(2, EquippedWeapon->GetLevel()));
		LevelUpWeaponButton->UnlockButton();
	}
}

void UInputButtonWidget::NativeOnInitialized()
{
	Super::NativeConstruct();
}

void UInputButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Click->OnClicked.AddDynamic(this, &UInputButtonWidget::OnButtonClicked);

	if (Resource)
	{
		Img_Icon->SetBrushResourceObject(Resource);
	}

	CooldownTimer = Cooldown;
	PB_Cooldown->SetPercent(1.0f);
	Txt_KeyLabel->SetText(KeyLabel);
	SetNeedValue(NeedValue);
}

void UInputButtonWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CooldownTimer += InDeltaTime;
	CooldownTimer = FMath::Clamp(CooldownTimer, 0.0f, Cooldown);
	PB_Cooldown->SetPercent(CooldownTimer / Cooldown);
	Img_Ready->SetVisibility(CanClick() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UInputButtonWidget::SetNeedValue(int32 InNeedValue)
{
	NeedValue = InNeedValue;
	Txt_NeedValue->SetText(FText::AsNumber(NeedValue));
}

bool UInputButtonWidget::CanClick()
{
	if (ClickCondition.IsBound() && ClickCondition.Execute() == false)
	{
		return false;
	}

	if (Btn_Click->GetIsEnabled() == false)
	{
		return false;
	}

	if (CooldownTimer < Cooldown
		|| NeedValue > GetPlayerController()->GetLocalPlayerState()->GetSoulAbsorptionCount())
	{
		return false;
	}

	return true;
}

void UInputButtonWidget::LockButton()
{
	Btn_Click->SetIsEnabled(false);
	PB_Cooldown->SetIsEnabled(false);

	PB_Cooldown->SetVisibility(ESlateVisibility::Hidden);
}

void UInputButtonWidget::UnlockButton()
{
	Btn_Click->SetIsEnabled(true);
	PB_Cooldown->SetIsEnabled(true);

	PB_Cooldown->SetVisibility(ESlateVisibility::Visible);
}

void UInputButtonWidget::OnButtonClicked()
{
	ReturnIfFalse(CanClick());

	CooldownTimer = 0.0f;
	OnClicked.Broadcast();
}
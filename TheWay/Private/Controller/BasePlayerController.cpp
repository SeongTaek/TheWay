// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BasePlayerController.h"
#include "TheWay.h"
#include "Utility/CreataekUtility.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UI/InGameHUD.h"
#include "UI/InGameHUDWidget.h"
#include "Framework/BasePlayerState.h"
#include "Interface/PickableActorInterface.h"
#include "Character/BaseCharacter.h"
#include "Framework/BasePlayerState.h"
#include "Framework/BaseGameStateBase.h"
#include "Item/Weapon/WeaponBase.h"
#include "Item/ItemBase.h"
#include "Item/Soul/SoulItem.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Framework/TheWayInGameState.h"

ABasePlayerController::ABasePlayerController()
{
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ABasePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ABasePlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//InputComponent->BindAction("Pick", IE_Pressed, this, &ABasePlayerController::OnPick);
}

void ABasePlayerController::ItemAcquired(const AItemBase* Item)
{
	if (Item->IsA(ASoulItem::StaticClass()))
	{
		if (ItemAcquiredSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ItemAcquiredSound, ViewCharacter->GetActorLocation(), ViewCharacter->GetActorRotation());
		}
	}
}

void ABasePlayerController::OnViewCharacterDeath()
{
	ReturnIfNull(ViewCharacter);

	// 보고있는 캐릭터가 죽으면 새로운 캐릭터로 이전한다.
	for (TActorIterator<ABaseCharacter> It(GetWorld()); It; ++It)
	{
		ABaseCharacter* BaseCharacter = *It;

		if (BaseCharacter == ViewCharacter ||
			BaseCharacter->IsDead() ||
			BaseCharacter->GetGenericTeamId() != ViewCharacter->GetGenericTeamId())
		{
			continue;;
		}

		Possess(BaseCharacter);
		SetViewCharacater(BaseCharacter);
		break;
	}
}

void ABasePlayerController::OnEquippedWeaponChanged(class AWeaponBase* LastWeapon, class AWeaponBase* EquippedWeapon)
{
	if (OnViewCharacterEquippedWeaponChanged.IsBound())
	{
		OnViewCharacterEquippedWeaponChanged.Broadcast(LastWeapon, EquippedWeapon);
	}
}

AInGameHUD* ABasePlayerController::GetInGameHUD() const
{
	return MyHUD ? Cast<AInGameHUD>(MyHUD) : nullptr;
}

UInGameHUDWidget* ABasePlayerController::GetInGameHUDWidget() const
{
	return GetInGameHUD() ? GetInGameHUD()->GetInGameHUDWidget() : nullptr;
}

class ATheWayInGameState* ABasePlayerController::GetInGameState() const
{
	return Cast<ATheWayInGameState>(GetWorld()->GetGameState());
}

ABasePlayerState* ABasePlayerController::GetLocalPlayerState() const
{
	ABaseGameStateBase* GameStateBase = Cast<ABaseGameStateBase>(GetWorld()->GetGameState());
	return (GameStateBase ? GameStateBase->GetLocalPlayerState() : nullptr);
}

const ABaseCharacter* ABasePlayerController::GetViewCharacter() const
{
	return ViewCharacter.Get();
}

void ABasePlayerController::SetViewCharacater(ABaseCharacter* TargetCharacter)
{
	ABaseCharacter* OldViewCharacter = ViewCharacter.Get();

	if (ViewCharacter.IsValid())
	{
		ViewCharacter->OnItemAcquired.RemoveAll(this);
		ViewCharacter->OnCharacterDeath.RemoveAll(this);
		ViewCharacter->OnEquippedWeaponChanged.RemoveAll(this);
	}
	ViewCharacter = TargetCharacter;

	ViewCharacter->OnItemAcquired.AddUObject(this, &ABasePlayerController::ItemAcquired);
	ViewCharacter->OnCharacterDeath.AddUObject(this, &ABasePlayerController::OnViewCharacterDeath);
	ViewCharacter->OnEquippedWeaponChanged.AddUObject(this, &ABasePlayerController::OnEquippedWeaponChanged);

	ViewCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = true;

	FViewTargetTransitionParams TransitionParams;
	TransitionParams.BlendFunction = EViewTargetBlendFunction::VTBlend_Cubic;
	TransitionParams.BlendTime = 1.0f;
	TransitionParams.BlendExp = 2.0f;

	SetViewTarget(Cast<AActor>(TargetCharacter), TransitionParams);

	if (OnViewCharacterChanged.IsBound())
	{
		OnViewCharacterChanged.Broadcast(OldViewCharacter, TargetCharacter);
	}
}

void ABasePlayerController::OnPick()
{
	if (PickedActor.IsValid())
	{
		Cast<IPickableActorInterface>(PickedActor)->EndPick();
		PickedActor.Reset();
	}

	FHitResult HitResult;
	if (GetHitResultUnderCursor(ECC_PICK, false, HitResult))
	{
		IPickableActorInterface* PickableActor = Cast<IPickableActorInterface>(HitResult.GetActor());
		if (PickableActor && PickableActor->IsPickable())
		{
			PickableActor->StartPick();
			PickedActor = HitResult.GetActor();
		}
	}

	OnPickActor.Broadcast(PickedActor);
}

void ABasePlayerController::ConsumeHealthPotion()
{
	ReturnIfFalse(ViewCharacter.IsValid());
	ReturnIfTrue(ViewCharacter->IsDead());
	ViewCharacter->ConsumeHealthPotion();
	if (ConsumeHealthPotionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ConsumeHealthPotionSound, ViewCharacter->GetActorLocation(), ViewCharacter->GetActorRotation());
	}
}

void ABasePlayerController::LevelUpWeapon()
{
	ReturnIfFalse(ViewCharacter.IsValid());
	ReturnIfTrue(ViewCharacter->IsDead());
	ViewCharacter->LevelUpWeapon();
	if (LevelUpWeaponSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LevelUpWeaponSound, ViewCharacter->GetActorLocation(), ViewCharacter->GetActorRotation());
	}
}

bool ABasePlayerController::CanLevelUpWeapon()
{
	ReturnFalseIfFalse(ViewCharacter.IsValid());
	ReturnFalseIfTrue(ViewCharacter->IsDead());
	ReturnFalseIfTrue(ViewCharacter->GetEquippedWeapon() == nullptr);

	return ViewCharacter->GetEquippedWeapon()->IsMaxLevel() == false;
}

void ABasePlayerController::UseSkill(int Index)
{
	ReturnIfFalse(ViewCharacter.IsValid());
	ReturnIfTrue(ViewCharacter->IsDead());
	ViewCharacter->UseSkill(Index);
}

bool ABasePlayerController::CanUseSkill() const
{
	ReturnFalseIfFalse(ViewCharacter.IsValid());
	return ViewCharacter->CanUseSkill();
}

bool ABasePlayerController::HasSkill(int Index)
{
	ReturnFalseIfFalse(ViewCharacter.IsValid());
	return ViewCharacter->HasSkill(Index);
}

void ABasePlayerController::Dash()
{
	ReturnIfFalse(ViewCharacter.IsValid());
	ReturnIfTrue(ViewCharacter->IsDead());
	return ViewCharacter->Dash();
}

bool ABasePlayerController::CanDash() const
{
	ReturnFalseIfFalse(ViewCharacter.IsValid());
	ReturnFalseIfTrue(ViewCharacter->IsDead());
	return ViewCharacter->CanDash();
}

bool ABasePlayerController::IsViewCharacterDead() const
{
	ReturnFalseIfFalse(ViewCharacter.IsValid());
	return ViewCharacter->IsDead();
}

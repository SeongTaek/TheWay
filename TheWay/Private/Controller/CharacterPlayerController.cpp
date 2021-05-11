// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/CharacterPlayerController.h"
#include "Framework/TheWayDeveloperSettings.h"
#include "Utility/CreataekUtility.h"
#include "Character/BaseCharacter.h"
#include "Item/Weapon/WeaponBase.h"
#include "Framework/TheWayInGameState.h"
#include "UI/GamePausePopup.h"

ACharacterPlayerController::ACharacterPlayerController()
{
	bShowMouseCursor = false;
	bPausePopupOpend = false;
}

void ACharacterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(SearchHostileCharacterTimer, this, &ACharacterPlayerController::TimerHandle_SearchHostileCharacter, SearchHostileCharacterInterval, true);
}

void ACharacterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ACharacterPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACharacterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		InputComponent->BindAxis("MoveForward", this, &ACharacterPlayerController::MoveForward);
		InputComponent->BindAxis("MoveRight", this, &ACharacterPlayerController::MoveRight);
		InputComponent->BindAxis("Turn", this, &ACharacterPlayerController::Turn);
		InputComponent->BindAxis("LookUp", this, &ACharacterPlayerController::LookUp);
		InputComponent->BindAction("Attack", IE_Pressed, this, &ACharacterPlayerController::Attack);
		InputComponent->BindAction("OpenPausePopup", IE_Pressed, this, &ACharacterPlayerController::OpenPausePopup);
	}
}

void ACharacterPlayerController::MoveForward(float AxisValue)
{
	ReturnIfNull(ViewCharacter);
	ViewCharacter->AddMovementInput(ViewCharacter->GetActorForwardVector(), AxisValue);
}

void ACharacterPlayerController::MoveRight(float AxisValue)
{
	ReturnIfNull(ViewCharacter);
	ViewCharacter->AddMovementInput(ViewCharacter->GetActorRightVector(), AxisValue);
}

void ACharacterPlayerController::Turn(float AxisValue)
{
	ReturnIfNull(ViewCharacter);
	ViewCharacter->AddControllerYawInput(AxisValue * GetDefault<UTheWayDeveloperSettings>()->MouseSensitivity);
}

void ACharacterPlayerController::LookUp(float AxisValue)
{
	ReturnIfNull(ViewCharacter);
	ViewCharacter->AddControllerPitchInput(AxisValue * GetDefault<UTheWayDeveloperSettings>()->MouseSensitivity);
}

void ACharacterPlayerController::Attack()
{
	ReturnIfNull(ViewCharacter);
	ReturnIfFalse(ViewCharacter->CanAttack());
	ViewCharacter->Attack();
}

void ACharacterPlayerController::OpenPausePopup()
{
	if (bPausePopupOpend)
	{
		return;
	}

	UGamePausePopup* PausePopup = GetInGameState()->OpenGamePausePopup();
	if (PausePopup)
	{
		bPausePopupOpend = true;

		PausePopup->OnPopupClosed.AddLambda([this]()
		{
			bPausePopupOpend = false;
		});
	}
}

void ACharacterPlayerController::OnViewCharacterDeath()
{
	Super::OnViewCharacterDeath();
}

void ACharacterPlayerController::TimerHandle_SearchHostileCharacter()
{
	ReturnIfNull(ViewCharacter);

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		ViewCharacter->GetActorLocation(),
		FQuat::Identity,
		ViewCharacter->GetAttackCollisionChannel(),
		FCollisionShape::MakeSphere(ViewCharacter->GetEquippedWeapon()->GetWeaponStatComponent()->GetAttackRange()),
		CollisionQueryParam);

	if (bResult)
	{
		for (const FOverlapResult& Result : OverlapResults)
		{
			AActor* Actor = Result.GetActor();
			if (Actor == false || Actor->IsPendingKill() || Actor->IsA(ABaseCharacter::StaticClass()) == false)
			{
				continue;
			}

			FVector Direction = Actor->GetActorLocation() - ViewCharacter->GetActorLocation();
			float Dot = FVector::DotProduct(ViewCharacter->GetActorForwardVector(), Direction);

			// 내적 값이 0.25보다 크면 타겟이 앞에 있어서 공격 발동!
			if (Dot > 0.25f)
			{
				Attack();
			}
		}
	}
}
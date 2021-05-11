// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "TheWay.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

APlayerCharacter::APlayerCharacter()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("USpringArmComponent"));
	if (SpringArmComponent)
	{
		SpringArmComponent->SetupAttachment(RootComponent);
		SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
		SpringArmComponent->SetRelativeRotation(FRotator(0.0f, -30.0f, 0.0f));
		SpringArmComponent->TargetArmLength = 400.0f;
		SpringArmComponent->bUsePawnControlRotation = true;
		SpringArmComponent->bInheritYaw = true;
		SpringArmComponent->bInheritPitch = true;
		SpringArmComponent->bInheritRoll = true;
		SpringArmComponent->bDoCollisionTest = true;
	}

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	if (Camera)
	{
		Camera->SetupAttachment(SpringArmComponent);
		Camera->bUsePawnControlRotation = true;
	}

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetMesh()->SetCollisionObjectType(ECC_FRIENDLY_PAWN);

	LifeSpan = false;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

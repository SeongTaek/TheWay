// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"
#include "TheWay.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Animation/BaseAnimInstance.h"
#include "Utility/CreataekUtility.h"
#include "Item/Weapon/WeaponBase.h"
#include "DataTable/DataTableManager.h"
#include "DataTable/TheWayDataTable.h"
#include "Projectile/ProjectileBase.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Controller/BaseAIController.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Controller/BaseAIController.h"
#include "Character/CreatureCharacter.h"
#include "Item/Weapon/RangedWeapon/RangedWeaponBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Item/ItemBase.h"
#include "Item/Soul/SoulItem.h"
#include "Framework/BasePlayerState.h"
#include "Framework/BaseGameStateBase.h"
#include "Framework/TheWayDeveloperSettings.h"

ABaseCharacter* ABaseCharacter::Spawn(AActor* Owner, TSharedPtr<const struct FCharacterData> CharacterData, FVector Location, FQuat Rotator)
{
	ReturnNullptrIfFalse(CharacterData.IsValid());

	UClass* GeneratedBP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *CU::MakeClassPath(CharacterData->Blueprint_File, &GetDefault<UTheWayDeveloperSettings>()->CharacterClassBasePath)));
	UWorld* World = Owner->GetWorld();
	FActorSpawnParameters SpawnParams;
	FTransform SpawnTransform;

	SpawnParams.Owner = Owner;
	SpawnParams.Instigator = Owner->GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	SpawnParams.bDeferConstruction = true;
	SpawnTransform.SetLocation(Location);
	SpawnTransform.SetRotation(Rotator);

	ABaseCharacter* Character = World->SpawnActor<ABaseCharacter>(GeneratedBP, SpawnParams);
	Character->SetCharacterID(CharacterData->Id);
	Character->FinishSpawning(SpawnTransform);

	return Character;
}

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterStatComponent = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("CharacterStat"));

	GetMesh()->SetCollisionObjectType(ECC_FRIENDLY_PAWN);

	IsAttacking = false;
	MaxCombo = 1;
	AttackEndComboState();

	bLifeSpan = true;
	LifeSpan = 3.0f;
	DashSpeed = 3500.0f;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	DirectionToMove = GetActorForwardVector();

	FAnimMontagePlayInfo MontagePlayInfo = AnimInstance->ChooseIntroAnimMontage();
	if (MontagePlayInfo.MontageToPlay)
	{
		PlayAnimMontage(MontagePlayInfo.MontageToPlay, MontagePlayInfo.PlayRate, MontagePlayInfo.Section);
		IsPlayingIntroAnim = true;
	}
	else
	{
		IsPlayingIntroAnim = false;
	}
}

void ABaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UnequipWeapon();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UBaseAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &ABaseCharacter::OnMontageEnded);
	}

	CharacterStatComponent->OnHPIsZero.AddUObject(this, &ABaseCharacter::OnDeath);

	SetGenericTeamId(FGenericTeamId(ETeamAttitude::Friendly));
}

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

	CharacterStatComponent->TakeDamage(FinalDamage);
	UpdateDamagedAnimation(FinalDamage, DamageEvent, EventInstigator, DamageCauser);
	BP_SetMaterialEffect(ECharacterMaterialEffect::DAMAGED);

	return FinalDamage;
}

void ABaseCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AnimInstance->GetAttackMontagePlayInfo().MontageToPlay)
	{
		IsAttacking = false;
		AttackEndComboState();

		if (OnAttackEnded.IsBound())
		{
			OnAttackEnded.Broadcast();
		}
	}
	else if (Montage == AnimInstance->GetSkillMontagePlayInfo().MontageToPlay)
	{
		if (AnimInstance->GetSkillMontagePlayInfo().IsFlying)
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		}
		IsCastingSkill = false;
	}
	else if (Montage == AnimInstance->GetIntroMontagePlayInfo().MontageToPlay)
	{
		IsPlayingIntroAnim = false;
	}
}

void ABaseCharacter::UpdateDamagedAnimation(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (IsDead() || Damage <= 0.0f)
	{
		return;
	}

	FAnimMontagePlayInfo MontagePlayInfo = AnimInstance->ChooseDamagedAnimMontage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (MontagePlayInfo.MontageToPlay)
	{
		PlayAnimMontage(MontagePlayInfo.MontageToPlay, MontagePlayInfo.PlayRate, MontagePlayInfo.Section);
	}
}

void ABaseCharacter::EquipWeapon(AWeaponBase* Weapon)
{
	ReturnIfNull(Weapon);

	auto LastWeapon = EquippedWeapon;

	if (LastWeapon)
	{
		LastWeapon->Unequip();
	}

	EquippedWeapon = Weapon;
	EquippedWeapon->OnEquip(this);

	if (OnEquippedWeaponChanged.IsBound())
	{
		OnEquippedWeaponChanged.Broadcast(LastWeapon, EquippedWeapon);
	}
}

void ABaseCharacter::UnequipWeapon()
{
	ReturnIfNull(EquippedWeapon);

	EquippedWeapon->Unequip();
	EquippedWeapon = nullptr;
}

void ABaseCharacter::Attack()
{
	ReturnIfFalse(CanAttack());

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

bool ABaseCharacter::CanAttack()
{
	ReturnFalseIfTrue(IsDead());
	ReturnFalseIfTrue(IsPlayingIntroAnim);
	ReturnFalseIfTrue(IsCastingSkill);
	ReturnFalseIfTrue(EquippedWeapon == nullptr);
	ReturnFalseIfTrue(IsAttacking);

	return true;
}

void ABaseCharacter::UseSkill(int32 Index)
{
	ReturnIfFalse(CanUseSkill());

	FAnimMontagePlayInfo MontagePlayInfo = AnimInstance->ChooseSkillAnimMontage(Index);
	if (MontagePlayInfo.MontageToPlay)
	{
		PlayAnimMontage(MontagePlayInfo.MontageToPlay, MontagePlayInfo.PlayRate, MontagePlayInfo.Section);

		if (MontagePlayInfo.IsFlying)
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		}

		IsCastingSkill = true;
	}
}

bool ABaseCharacter::CanUseSkill()
{
	ReturnFalseIfTrue(IsDead());
	ReturnFalseIfTrue(IsCastingSkill);
	ReturnFalseIfTrue(GetCharacterMovement()->PendingLaunchVelocity.IsZero() == false);

	return true;
}

bool ABaseCharacter::HasSkill(int32 Index)
{
	return AnimInstance->ChooseSkillAnimMontage(Index).MontageToPlay != nullptr;
}

void ABaseCharacter::Dash()
{
	ReturnIfFalse(CanDash());

	FVector LaunchVelocity = GetVelocity().GetSafeNormal();

	LaunchVelocity.X *= DashSpeed;
	LaunchVelocity.Y *= DashSpeed;
	LaunchVelocity.Z = 0.0f;

	LaunchCharacter(LaunchVelocity, false, false);

	if (DashTrailEffect)
	{
		CU::SpawnEffect(DashTrailEffect, GetTransform(), this);
	}
}

bool ABaseCharacter::CanDash()
{
	ReturnFalseIfTrue(IsDead());
	ReturnFalseIfTrue(IsPlayingIntroAnim);
	ReturnFalseIfTrue(IsCastingSkill);
	ReturnFalseIfTrue(GetCharacterMovement()->PendingLaunchVelocity.IsZero() == false);
	ReturnFalseIfTrue(GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Walking);

	return true;
}

void ABaseCharacter::AcquireItem(class AItemBase* Item)
{
	if (Item->IsA(ASoulItem::StaticClass()))
	{
		BP_SetMaterialEffect(ECharacterMaterialEffect::ACQUIRE_ITEM);
	}

	if (OnItemAcquired.IsBound())
	{
		OnItemAcquired.Broadcast(Item);
	}
}

void ABaseCharacter::ConsumeHealthPotion()
{
	GetCharacterStatComponent()->AddHp(100.0f);
	BP_SetMaterialEffect(ECharacterMaterialEffect::CONSUME_HEALTH_POTION);

	if (HealthHealedSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HealthHealedSound, GetActorLocation(), GetActorRotation());
	}
}

void ABaseCharacter::LevelUpWeapon()
{
	GetEquippedWeapon()->LevelUp();
	BP_SetMaterialEffect(ECharacterMaterialEffect::LEVELUP_ATTACK_DAMAGE);

	if (WeaponLevelUpSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, WeaponLevelUpSound, GetActorLocation(), GetActorRotation());
	}
}

void ABaseCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;

	ReturnIfFalse(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));

	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void ABaseCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

/// <summary>
/// Overlap 이벤트 발생하는 충돌체
/// </summary>
/// <returns></returns>
ECollisionChannel ABaseCharacter::GetMeleeCollisionChannel() const
{
	switch (TeamId)
	{
	case ETeamAttitude::Friendly:
		return ECC_FRIENDLY_OVERLAP;
	case ETeamAttitude::Hostile:
		return ECC_HOSTILE_OVERLAP;
	case ETeamAttitude::Neutral:
	default:
		return ECC_NEUTRAL_OVERLAP;
	}
}

/// <summary>
/// Hit 이벤트 발생하는 충돌체
/// </summary>
/// <returns></returns>
ECollisionChannel ABaseCharacter::GetProjectileCollisionChannel() const
{
	switch (TeamId)
	{
	case ETeamAttitude::Friendly:
		return ECC_FRIENDLY_COLLISION;
	case ETeamAttitude::Hostile:
		return ECC_HOSTILE_COLLISION;
	case ETeamAttitude::Neutral:
	default:
		return ECC_NEUTRAL_COLLISION;
	}
}

ECollisionChannel ABaseCharacter::GetAttackCollisionChannel() const
{
	return (Cast<ARangedWeaponBase>(EquippedWeapon) ? GetProjectileCollisionChannel() : GetMeleeCollisionChannel());
}

int32 ABaseCharacter::GetAggroPoint(const ABaseCharacter* EnemyCharacter)
{
	float AggroPoint = 0;

	// 500 단위로 어그로 포인트 증가
	int32 Distance = FVector::Distance(GetActorLocation(), EnemyCharacter->GetActorLocation()) / 500;
	Distance = FMath::Clamp(Distance, 1, TNumericLimits<int32>::Max());
	AggroPoint += 100.0f / Distance;

	// 체력 어그로 포인트 증가
	AggroPoint += 100.0f / CharacterStatComponent->GetCurrentHp();

	return (int)AggroPoint;
}

float ABaseCharacter::GetFinalAttckDamage() const
{
	float WeaponAttackDamage = (GetEquippedWeapon() ? GetEquippedWeapon()->GetWeaponStatComponent()->GetAttackDamage() : 0.0f);
	return GetCharacterStatComponent()->GetAttackDamage() + WeaponAttackDamage;
}

class ABasePlayerState* ABaseCharacter::GetLocalPlayerState()
{
	ABaseGameStateBase* GameStateBase = Cast<ABaseGameStateBase>(GetWorld()->GetGameState());
	return (GameStateBase ? GameStateBase->GetLocalPlayerState() : nullptr);
}

void ABaseCharacter::SetCharacterID(int32 Id, bool bUpdateProperties)
{
	CharacterId = Id;
	CacheCharacterCsv();

	if (bUpdateProperties)
	{
		if (CharacterDataCsv.IsValid())
		{
			CharacterStatComponent->SetStatsID(CharacterDataCsv->Stats_Id);

			auto WeaponData = UDataTableManager::Get(this)->GetTableCsv<FWeaponData>(CharacterDataCsv->Id);
			if (WeaponData.IsValid())
			{
				if (EquippedWeapon == nullptr || EquippedWeapon->GetItemID() != WeaponData->Id)
				{
					if (EquippedWeapon)
					{
						UnequipWeapon();
					}
					
					if (AWeaponBase* DefaultWeapon = AWeaponBase::Spawn(this, WeaponData))
					{
						EquipWeapon(DefaultWeapon);
					}
				}
			}
		}
	}
}

void ABaseCharacter::OnDeath()
{
	HealthState = ECharacterHealthState::DEATH;
	SetActorEnableCollision(false);
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	if (auto AIController = Cast<ABaseAIController>(Controller))
	{
		AIController->StopAI();
	}

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

void ABaseCharacter::CacheCharacterCsv()
{
	ReturnIfNull(UDataTableManager::Get(this));
	CharacterDataCsv = UDataTableManager::Get(this)->GetTableCsv<FCharacterData>(CharacterId);
}

void ABaseCharacter::DropItems()
{
	ReturnIfFalse(CharacterDataCsv.IsValid());

	ABasePlayerState* BasePlayerState = GetLocalPlayerState();
	ReturnIfNull(BasePlayerState);

	// 드랍 아이템 확률 돌리기
	TArray<int> DropItemIdxList;
	for (int i = 0; i < CharacterDataCsv->DropItemList.Num(); ++i)
	{
		if (FMath::RandRange(1, 100) < CharacterDataCsv->DropItemList[i].Probability)
		{
			DropItemIdxList.Add(i);
		}
	}

	// 아이템 드랍
	for (int i = 0; i < DropItemIdxList.Num(); ++i)
	{
		const FDropItemData& DropItemData = CharacterDataCsv->DropItemList[DropItemIdxList[i]];
		auto ItemData = UDataTableManager::Get(this)->GetTableCsv<FItemData>(DropItemData.Id);
		if (ItemData.IsValid())
		{
			const int ItemNum = FMath::RandRange(DropItemData.MinDrop, DropItemData.MaxDrop);
			for (int j = 0; j < ItemNum; ++j)
			{
				AItemBase* Item = AItemBase::Spawn(this, ItemData);
				Item->BeItemAcquired.AddUObject(BasePlayerState, &ABasePlayerState::AcquireItem);
				if (ABaseCharacter* InstigatorCharacter = Cast<ABaseCharacter>(GetInstigator()))
				{
					Item->BeItemAcquired.AddUObject(InstigatorCharacter, &ABaseCharacter::AcquireItem);
				}

				if (Item->IsA(ASoulItem::StaticClass()))
				{
					FVector SlopeControlDirection = GetActorForwardVector().RotateAngleAxis(-135.0f, GetActorRightVector());
					FVector SlopeControlPoint = SlopeControlDirection.RotateAngleAxis(j * (360.0f / (float)ItemNum), GetActorForwardVector()) * 2000.0f;
					Cast<ASoulItem>(Item)->SetSlopeControlPoint(SlopeControlPoint);
				}
			}
		}
	}
}

void ABaseCharacter::SetGenericTeamId(const FGenericTeamId& NewTeamId)
{
	TeamId = NewTeamId;

	GetMesh()->SetCollisionProfileName(CU::GetPawnCollisionProfileByTeamId(TeamId));
	GetCapsuleComponent()->SetCollisionProfileName(CU::GetPawnCollisionProfileByTeamId(TeamId));

	if (EquippedWeapon)
	{
		EquippedWeapon->SetGenericTeamId(TeamId);
	}
}

void ABaseCharacter::StartPick()
{
	TArray<UMeshComponent*> MeshComponents;
	GetComponents(MeshComponents);

	for (UMeshComponent* MeshComp : MeshComponents)
	{
		MeshComp->bRenderCustomDepth = 1;
		MeshComp->CustomDepthStencilValue = 2;
		MeshComp->MarkRenderStateDirty();
	}
}

void ABaseCharacter::EndPick()
{
	TArray<UMeshComponent*> MeshComponents;
	GetComponents(MeshComponents);

	for (UMeshComponent* MeshComp : MeshComponents)
	{
		MeshComp->bRenderCustomDepth = 0;
		MeshComp->CustomDepthStencilValue = 0;
		MeshComp->MarkRenderStateDirty();
	}
}
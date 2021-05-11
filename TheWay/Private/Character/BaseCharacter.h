// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "Interface/PickableActorInterface.h"
#include "Components/CharacterStatComponent.h"
#include "BaseCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEnded);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEquippedWeaponChanged, class AWeaponBase*, class AWeaponBase*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemAcquired, const class AItemBase*);
DECLARE_MULTICAST_DELEGATE(FOnCharacterDeath);

UENUM(BlueprintType)
enum class ECharacterMaterialEffect : uint8
{
	ACQUIRE_ITEM = 0,
	CONSUME_HEALTH_POTION = 1,
	LEVELUP_ATTACK_DAMAGE = 2,
	DAMAGED = 3,
};

UENUM(BlueprintType)
enum class ECharacterHealthState : uint8
{
	HEALTHY = 0,
	DEATH = 1,
};

USTRUCT(BlueprintType)
struct FCharacterDebuffInfo
{
	GENERATED_BODY()
	
public:
	class ABaseCharacter* OwnerCharacter;

	UPROPERTY()
	uint8 CannotMoveCount;
};

UCLASS()
class ABaseCharacter : public ACharacter, public IGenericTeamAgentInterface, public IPickableActorInterface
{
	GENERATED_BODY()

public:
	static ABaseCharacter* Spawn(AActor* Owner, TSharedPtr<const struct FCharacterData> CharacterData, FVector Location, FQuat Rotator);

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	virtual void OnMontageEnded(class UAnimMontage* Montage, bool bInterrupted);
	virtual void UpdateDamagedAnimation(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	virtual void EquipWeapon(class AWeaponBase* Weapon);
	virtual void UnequipWeapon();
	virtual void Attack();
	virtual bool CanAttack();
	virtual void UseSkill(int32 Index);
	virtual bool CanUseSkill();
	virtual bool HasSkill(int32 Index);
	virtual void Dash();
	virtual bool CanDash();
	UFUNCTION()
	virtual void AcquireItem(class AItemBase* Item);
	virtual void ConsumeHealthPotion();
	virtual void LevelUpWeapon();

	void AttackStartComboState();
	void AttackEndComboState();

	bool IsDead() const { return HealthState == ECharacterHealthState::DEATH; }

	const class UBaseAnimInstance* GetAnimInstance() const { return AnimInstance; }
	class AWeaponBase* GetEquippedWeapon() const { return EquippedWeapon; }
	class UCharacterStatComponent* GetCharacterStatComponent() const { return CharacterStatComponent ; }
	ECollisionChannel GetMeleeCollisionChannel() const;
	ECollisionChannel GetProjectileCollisionChannel() const;
	ECollisionChannel GetAttackCollisionChannel() const;
	int32 GetAggroPoint(const ABaseCharacter* EnemyCharacter);
	float GetFinalAttckDamage() const;
	class ABasePlayerState* GetLocalPlayerState();

	bool GetIsAttacking() const { return IsAttacking; }
	bool GetIsCastingSkill() const { return IsCastingSkill; }

	void SetCharacterID(int32 Id, bool bUpdateProperties = true);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_SetMaterialEffect(ECharacterMaterialEffect CharacterMaterialEffect);

	FOnEquippedWeaponChanged OnEquippedWeaponChanged;
	FOnAttackEnded OnAttackEnded;
	FOnItemAcquired OnItemAcquired;
	FOnCharacterDeath OnCharacterDeath;

protected:
	virtual void OnDeath();
	virtual void CacheCharacterCsv();

	void DropItems();

protected:
	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UCharacterStatComponent* CharacterStatComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sounds)
	class USoundBase* DamagedSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sounds)
	class USoundBase* HealthHealedSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sounds)
	class USoundBase* WeaponLevelUpSound;

	UPROPERTY()
	class UBaseAnimInstance* AnimInstance;

	UPROPERTY()
	ECharacterHealthState HealthState;

	UPROPERTY(BlueprintReadOnly)
	class AWeaponBase* EquippedWeapon;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack)
	bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack)
	bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack)
	int CurrentCombo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Attack)
	int MaxCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	bool IsCastingSkill;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Life)
	bool bLifeSpan;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Life)
	float LifeSpan;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation)
	bool IsPlayingIntroAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Dash)
	float DashSpeed;

	UPROPERTY(EditDefaultsOnly, Category = Effect)
	TSubclassOf<class AActor> DashTrailEffect;

	int32 CharacterId;

	TSharedPtr<const struct FCharacterData> CharacterDataCsv;

protected:
	FVector DirectionToMove = FVector::ForwardVector;

	/*
	* 특수 상태(디버프)
	*/
public:
	UFUNCTION(BlueprintPure, Category = "Character")
	FORCEINLINE bool IsMovable() const { return (DebuffInfo.CannotMoveCount == 0); }

	UPROPERTY()
	FCharacterDebuffInfo DebuffInfo;

	/*
	* IGenericTeamAgentInterface
	*/
public:
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamId) override;
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }

protected:
	UPROPERTY()
	FGenericTeamId TeamId;

	/*
	* IPickableActorInterface
	*/
public:
	virtual void StartPick() override;
	virtual void EndPick() override;
};

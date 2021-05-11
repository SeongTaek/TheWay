// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapon/WeaponBase.h"
#include "Utility/CreataekUtility.h"
#include "Framework/TheWayDeveloperSettings.h"
#include "Character/BaseCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WeaponStatComponent.h"
#include "Projectile/ProjectileBase.h"
#include "TheWay.h"
#include "DataTable/DataTableManager.h"
#include "DataTable/TheWayDataTable.h"

AWeaponBase* AWeaponBase::Spawn(AActor* Owner, TSharedPtr<const struct FWeaponData> WeaponData)
{
	ReturnNullptrIfFalse(WeaponData.IsValid());

	UClass* GeneratedBP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *CU::MakeClassPath(WeaponData->Blueprint_File, &GetDefault<UTheWayDeveloperSettings>()->WeaponClassBasePath)));
	UWorld* World = Owner->GetWorld();
	FActorSpawnParameters SpawnParams;
	FTransform SpawnTransform;

	SpawnParams.Owner = Owner;
	SpawnParams.Instigator = Owner->GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.bDeferConstruction = true;
	SpawnTransform.SetLocation(Owner->GetActorLocation());
	SpawnTransform.SetRotation(Owner->GetActorRotation().Quaternion());

	AWeaponBase* Weapon = World->SpawnActor<AWeaponBase>(GeneratedBP, SpawnParams);
	auto WeaponStatsData = UDataTableManager::Get(Owner)->GetTableCsv<FWeaponStatsData>(TPair<int32, int32>(WeaponData->Stats_Id, 1));
	Weapon->SetItemID(WeaponData->Id);
	Weapon->WeaponStatComponent->SetStatsData(WeaponStatsData);

	Weapon->FinishSpawning(SpawnTransform);

	return Weapon;
}

AWeaponBase::AWeaponBase()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	RootComponent = Mesh;

	WeaponStatComponent = CreateDefaultSubobject<UWeaponStatComponent>(TEXT("WeaponStat"));
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponStatComponent->GetAutomaticAttack())
	{
		GetWorldTimerManager().SetTimer(AutomaticAttackTimer, this, &AWeaponBase::DecideAttack, WeaponStatComponent->GetAutomaticAttackSpeed(), true);
	}
}

void AWeaponBase::StartAttack()
{
}

void AWeaponBase::DecideAttack()
{
}

void AWeaponBase::OnEquip(ABaseCharacter* InOwnerCharacter)
{
	SetOwnerCharacter(InOwnerCharacter);
	AttachToOwnerCharacter();
}

void AWeaponBase::Unequip()
{
	Destroy();
}

void AWeaponBase::LevelUp()
{
	const FWeaponData* WeaponData = static_cast<const FWeaponData*>(ItemDataCsv.Get());
	ReturnIfNull(WeaponData);
	ReturnIfFalse(Level < WeaponData->MaxLevel);

	++Level;
	auto WeaponStatsData = UDataTableManager::Get(this)->GetTableCsv<FWeaponStatsData>(TPair<int32, int32>(ItemDataCsv->Id, Level));
	WeaponStatComponent->SetStatsData(WeaponStatsData);

	OnWeaponLevelUp.Broadcast();
}

bool AWeaponBase::IsMaxLevel()
{
	ReturnFalseIfFalse(ItemDataCsv.IsValid());

	const FWeaponData* WeaponData = static_cast<const FWeaponData*>(ItemDataCsv.Get());
	ReturnFalseIfNull(WeaponData);

	return Level >= WeaponData->MaxLevel;
}

void AWeaponBase::AttachToOwnerCharacter()
{
	ReturnIfNull(OwnerCharacter);

	Mesh->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, AttachedSocketName);
}

float AWeaponBase::GetFinalAttackDamage() const
{
	ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner());
	float FinalDamage = (Character ? Character->GetFinalAttckDamage() : 0.0f);
	return FinalDamage;
}

void AWeaponBase::SetItemID(int32 Id, bool bUpdateProperties)
{
	Super::SetItemID(Id, bUpdateProperties);
}

void AWeaponBase::CacheItemCsv()
{
	ReturnIfNull(UDataTableManager::Get(this));
	ItemDataCsv = UDataTableManager::Get(this)->GetTableCsv<FWeaponData>(ItemId);
}

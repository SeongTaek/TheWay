// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemBase.h"
#include "Utility/CreataekUtility.h"
#include "Framework/TheWayDeveloperSettings.h"
#include "DataTable/DataTableManager.h"
#include "DataTable/TheWayDataTable.h"

AItemBase* AItemBase::Spawn(AActor* Owner, TSharedPtr<const struct FItemData> ItemData)
{
	ReturnNullptrIfFalse(ItemData.IsValid());

	UClass* GeneratedBP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *CU::MakeClassPath(ItemData->Blueprint_File, &GetDefault<UTheWayDeveloperSettings>()->ItemClassBasePath)));
	UWorld* World = Owner->GetWorld();
	FActorSpawnParameters SpawnParams;
	FTransform SpawnTransform;

	SpawnParams.Owner = Owner;
	SpawnParams.Instigator = Owner->GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.bDeferConstruction = true;
	SpawnTransform.SetLocation(Owner->GetActorLocation());
	SpawnTransform.SetRotation(Owner->GetActorRotation().Quaternion());

	AItemBase* Item = World->SpawnActor<AItemBase>(GeneratedBP, SpawnParams);
	Item->SetItemID(ItemData->Id);
	Item->FinishSpawning(SpawnTransform);

	return Item;
}

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBase::BeAcquired()
{
	BeItemAcquired.Broadcast(this);
}

int32 AItemBase::GetItemID() const
{
	return ItemId;
}

void AItemBase::SetItemID(int32 Id, bool bUpdateProperties /*= true*/)
{
	ItemId = Id;
	CacheItemCsv();
}

void AItemBase::CacheItemCsv()
{
	ReturnIfNull(UDataTableManager::Get(this));
	ItemDataCsv = UDataTableManager::Get(this)->GetTableCsv<FItemData>(ItemId);
}


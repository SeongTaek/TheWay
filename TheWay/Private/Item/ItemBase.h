// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FBeItemAcquired, class AItemBase*);

UCLASS()
class AItemBase : public AActor
{
	GENERATED_BODY()
	
public:
	static AItemBase* Spawn(AActor* Owner, TSharedPtr<const struct FItemData> ItemData);
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void BeAcquired();

	virtual int32 GetItemID() const;
	virtual void SetItemID(int32 Id, bool bUpdateProperties = true);

protected:
	virtual void CacheItemCsv();

public:
	FBeItemAcquired BeItemAcquired;

protected:
	UPROPERTY(Transient)
	class ABaseCharacter* OwnerCharacter;

	int32 ItemId;
	TSharedPtr<const struct FItemData> ItemDataCsv;

public:
	FORCEINLINE class ABaseCharacter* GetOwnerCharacter() const { return OwnerCharacter; }
	FORCEINLINE void SetOwnerCharacter(class ABaseCharacter* InOwnerCharacter) { OwnerCharacter = InOwnerCharacter; }
};

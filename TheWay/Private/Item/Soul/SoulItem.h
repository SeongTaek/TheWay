// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "SoulItem.generated.h"

/**
 * 
 */
UCLASS()
class ASoulItem : public AItemBase
{
	GENERATED_BODY()

public:
	static ASoulItem* Spawn(AActor* Owner, TSharedPtr<const struct FItemData> ItemData);

public:
	// Sets default values for this actor's properties
	ASoulItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveToInstigator(const float DeltaTime);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void BeAcquired() override;

	virtual void SetItemID(int32 Id, bool bUpdateProperties = true) override;

	void SetSlopeControlPoint(FVector InSlopeControlPoint);

protected:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere, Category = Collision)
	class USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, Category = Particle)
	class UParticleSystemComponent* ParticleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Particles)
	class UParticleSystem* AbsorptionParticle;

	UPROPERTY(EditAnywhere, Category = Movement, meta = (ClampMin = 0.1))
	float Duration;

	UPROPERTY(EditAnywhere, Category = Movement)
	float DelayTime;

	FVector SlopeControlPoint;
	FVector OriginLocation;
	float LerpRatio;
	float ElapsedTime;
};

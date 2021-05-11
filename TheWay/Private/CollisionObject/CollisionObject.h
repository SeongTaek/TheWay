// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenericTeamAgentInterface.h"
#include "CollisionObject.generated.h"

UCLASS()
class ACollisionObject : public AActor, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

#if !(UE_BUILD_SHIPPING)
public:
	static bool bDrawDebugInfo;
#endif

public:
	struct FActorDamageEvent
	{
		AActor* Victim;
		FDamageEvent* DamageEvent;
	};

public:
	// Sets default values for this actor's properties
	ACollisionObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* 
	* IGenericTeamAgentInterface
	*/
public:
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamId) override { TeamId = NewTeamId; }
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }

protected:
	FGenericTeamId TeamId;

protected:
	void QueryPointDamageEvent(const float Damage, const FHitResult& HitResult, FPointDamageEvent& OutDamageEvent);
	void QueryRadialDamageEvent(float BaseDamage, const FVector& Origin, const FRotator& Rotation, float Radius, const TArray<AActor*>& IgnoreActors, ECollisionChannel TraceChannel, TMap<AActor*, FRadialDamageEvent>& OutOverlapInfoMap);
	void QueryOverlapHitResults(const FVector& Origin, const FRotator& Rotation, const TArray<AActor*>& IgnoreActors, const FCollisionShape& CollisionShape, ECollisionChannel TraceChannel, TMap<AActor*, TArray<FHitResult>>& OutOverlapHitResults);
	bool QueryHitResult(UPrimitiveComponent* TargetComp, FVector const& Origin, const TArray<AActor*>& IgnoreActors, ECollisionChannel TraceChannel, FHitResult& OutHitResult);

	void HandleDamageAction(TArray<FActorDamageEvent>& ActorDamageEvents, AActor* OverrideDealer = nullptr);
};

UCLASS()
class APeriodicDamageSphereCollision: public ACollisionObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APeriodicDamageSphereCollision();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void ApplyDamage();

	float GetFinalAttackDamage();

protected:
	UPROPERTY(EditAnywhere)
	class USphereComponent* Collision;
	
	UPROPERTY(EditAnywhere)
	float Damage;

	UPROPERTY(EditAnywhere)
	float KnockbackPower;

	UPROPERTY(EditAnywhere)
	float DurationTime;

	UPROPERTY(EditAnywhere)
	float PeriodTime;

	class ABaseCharacter* OwnerCharacter;
	FTimerHandle TimerHandle_NextDamage;
	float ElapseTime;
};

USTRUCT(BlueprintType)
struct FImpulseDamageEvent : public FPointDamageEvent
{
	GENERATED_BODY()

	/** ID for this class. NOTE this must be unique for all damage events. */
	static const int32 ClassID = 99999;

	virtual int32 GetTypeID() const override { return FImpulseDamageEvent::ClassID; };
	virtual bool IsOfType(int32 InID) const override { return (FImpulseDamageEvent::ClassID == InID) || FPointDamageEvent::IsOfType(InID); };
};
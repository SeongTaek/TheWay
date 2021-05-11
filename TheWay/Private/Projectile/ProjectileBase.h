// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollisionObject/CollisionObject.h"
#include "ProjectileBase.generated.h"

/**
 * 
 */
UCLASS()
class AProjectileBase : public ACollisionObject
{
	GENERATED_BODY()
	
public:
	AProjectileBase();

public: 
	virtual void Shoot(FVector ShootDirection, float InitialSpeed);

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

	float GetFinalAttackDamage() const;

	UFUNCTION()
	void OnCollided(const FHitResult& HitResult);

	UFUNCTION()
	void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

	UFUNCTION()
	void Explode(const FVector& ImpactPosition, const FRotator& ImpactRotation);

	UFUNCTION()
	void TimerHandle_LifeTime();

	UFUNCTION()
	void TimerHandle_SearchHomingTarget();

public:
	UPROPERTY(VisibleDefaultsOnly, Category = Collision)
	class UCapsuleComponent* Body;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
	class UProjectileMovementComponent* Movement;

	/*
	* IGenericTeamAgentInterface
	*/
public:
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamId) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats)
	float AttackDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats)
	float DamageRadius;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
	float KnockbackPower;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Life)
	float ProjectileLifeSpan;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Homming)
	float SearchHomingTargetInterval = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Homming)
	float SearchHomingTargetRadius = 2000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Particles)
	class UParticleSystem* HitParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Particles)
	class UParticleSystem* NoHitParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sounds)
	class USoundBase* ExplosionSound;

	FVector ShotDirection;

	FTimerHandle LifeSpanTimer;
	FTimerHandle SearchHomingTargetTimer;

	ECollisionChannel ECCOverlapObjectType;
};

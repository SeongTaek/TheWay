// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/TheWayAnimNotify.h"
#include "Character/BaseCharacter.h"
#include "Item/Weapon/WeaponBase.h"
#include "Projectile/ProjectileBase.h"
#include "Controller/BaseAIController.h"
#include "CollisionObject/CollisionObject.h"

void UTheWayAnimNotify_WeaponAttack::Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation)
{
	Super::Notify(Mesh, Animation);

	if (ABaseCharacter* Owner = Cast<ABaseCharacter>(Mesh->GetOwner()))
	{
		if (AWeaponBase* Weapon = Owner->GetEquippedWeapon())
		{
			if (Owner->IsLocallyControlled())
			{
				Weapon->DecideAttack();
			}
		}
	}
}

void UTheWayAnimNotify_SpawnProjectile::Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation)
{
	Super::Notify(Mesh, Animation);

	if (ABaseCharacter* Character = Cast<ABaseCharacter>(Mesh->GetOwner()))
	{
		for (const FProjectileSet_AnimNotify_SpawnProjectile& Projectile : ProjectileArray)
		{
			FQuat FinalQuaternion = Character->GetActorQuat() * FQuat::MakeFromEuler(Projectile.ShootAngleOffset);
			FVector FinalShootDirection = FinalQuaternion.GetForwardVector();
			FQuat BetweenQuat = FQuat::FindBetweenVectors(FVector::ForwardVector, Character->GetActorForwardVector());
			FVector BetweenAxis = BetweenQuat.GetRotationAxis();
			float BetweenAngle = FMath::RadiansToDegrees(BetweenQuat.GetAngle());
			FVector FinalLocationOffset = Projectile.LocationOffset.RotateAngleAxis(BetweenAngle, BetweenAxis);
			FVector FinalLocation = (Projectile.LocationSocketName.IsNone() ? Character->GetActorLocation() : Character->GetMesh()->GetSocketLocation(Projectile.LocationSocketName)) + FinalLocationOffset;
			FActorSpawnParameters SpawnParams;

			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.Instigator = Character;

			AProjectileBase* SpawnedProjectile = Character->GetWorld()->SpawnActor<AProjectileBase>(Projectile.ProjectileClass, FinalLocation, FRotator(0.0f), SpawnParams);
			SpawnedProjectile->Shoot(FinalShootDirection, Projectile.ProjectileInitialSpeed);
		}
	}
}

void UTheWayAnimNotify_SpawnCollisionObject::Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation)
{
	Super::Notify(Mesh, Animation);

	if (UClass* SpawnClass = CollisionObjectClass.TryLoadClass<ACollisionObject>())
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(Mesh->GetOwner()))
		{
			UWorld* World = Character->GetWorld();
			FActorSpawnParameters SpawnParams;

			SpawnParams.Owner = Character;
			SpawnParams.Instigator = Character->GetInstigator();
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			ACollisionObject* CollisionObject = World->SpawnActor<ACollisionObject>(SpawnClass, Character->GetActorLocation() + SpawnLocationOffset, (Character->GetActorRotation() + SpawnRotationOffset), SpawnParams);
		}
	}
}

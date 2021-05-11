// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/Soul/SoulItem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Utility/CreataekUtility.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DataTable/DataTableManager.h"
#include "DataTable/TheWayDataTable.h"
#include "Kismet/GameplayStatics.h"

ASoulItem* ASoulItem::Spawn(AActor* Owner, TSharedPtr<const struct FItemData> ItemData)
{
	ReturnNullptrIfFalse(ItemData.IsValid());

	UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, *ItemData->Blueprint_File));
	UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
	UWorld* World = Owner->GetWorld();
	FActorSpawnParameters SpawnParams;
	FTransform SpawnTransform;

	SpawnParams.Owner = Owner;
	SpawnParams.Instigator = Owner->GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.bDeferConstruction = true;
	SpawnTransform.SetLocation(Owner->GetActorLocation());
	SpawnTransform.SetRotation(Owner->GetActorRotation().Quaternion());

	ASoulItem* Item = World->SpawnActor<ASoulItem>(GeneratedBP->GeneratedClass, SpawnParams);
	Item->SetItemID(ItemData->Id);
	Item->FinishSpawning(SpawnTransform);

	return Item;
}

ASoulItem::ASoulItem()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SphereComponent->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASoulItem::OnBeginOverlap);
	RootComponent = SphereComponent;

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	ParticleComponent->SetupAttachment(RootComponent);
}

void ASoulItem::BeginPlay()
{
	Super::BeginPlay();

	SlopeControlPoint = FVector::ZeroVector;
	OriginLocation = GetActorLocation();
	LerpRatio = 0.0f;
	ElapsedTime = 0.0f;
}

void ASoulItem::MoveToInstigator(const float DeltaTime)
{
	FVector NewActorLocation = FMath::CubicInterp(OriginLocation, SlopeControlPoint, GetInstigator()->GetActorLocation(), FVector::ZeroVector, LerpRatio);
	FVector NewForwrad = (NewActorLocation - GetActorLocation()).GetSafeNormal();
	SetActorRotation(NewForwrad.Rotation());
	SetActorLocation(NewActorLocation);

	LerpRatio = FMath::Clamp(LerpRatio + DeltaTime / Duration, 0.0f, 1.0f);
}

void ASoulItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 대상이 없어지면 제거
	if (GetInstigator() == nullptr)
	{
		BeAcquired();
		return;
	}

	ElapsedTime += DeltaTime;

	if (ElapsedTime > DelayTime)
	{
		MoveToInstigator(DeltaTime);
	}
}

void ASoulItem::BeAcquired()
{
	Super::BeAcquired();

	if (AbsorptionParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AbsorptionParticle, GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f), true);
	}

	Destroy();
}

void ASoulItem::SetItemID(int32 Id, bool bUpdateProperties)
{
	Super::SetItemID(Id, bUpdateProperties);
}

void ASoulItem::SetSlopeControlPoint(FVector InSlopeControlPoint)
{
	SlopeControlPoint = InSlopeControlPoint;
}

void ASoulItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetInstigator() == OtherActor)
	{
		BeAcquired();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickableActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickableActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IPickableActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool IsPickable() const { return true; }
	virtual void StartPick() = 0;
	virtual void EndPick() = 0;
};

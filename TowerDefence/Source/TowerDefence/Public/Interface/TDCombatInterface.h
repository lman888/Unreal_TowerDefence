// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TDCombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTDCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TOWERDEFENCE_API ITDCombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION()
	virtual void HandleDeath() = 0;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * TD GameplayTags
 * Singleton containing native GameplayTags
 */
struct FTDGameplayTags
{
public:

 static const FTDGameplayTags& Get() { return GameplayTags; }
 
 static void InitializeNativeGameplayTags();

 FGameplayTag Health;
 FGameplayTag AttackSpeed;
 FGameplayTag RangedArmour;
 FGameplayTag MovementSpeed;
 FGameplayTag AttackRange;
 
 FGameplayTag Damage;
 
protected:

private:

 static FTDGameplayTags GameplayTags;
 
};

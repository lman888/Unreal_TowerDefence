// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TDAbilitySystemComponent.generated.h"

class UTDAttributeSet;
/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UTDAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AddCharacterAbility(TSubclassOf<UGameplayAbility>& Ability);

	void UpgradeAbility(TSubclassOf<UGameplayAbility>& Ability) const;
};
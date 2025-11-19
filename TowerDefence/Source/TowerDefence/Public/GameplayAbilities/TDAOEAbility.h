// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/TDGameplayAbility.h"
#include "TDAOEAbility.generated.h"

/**
 *
 */
UCLASS()
class TOWERDEFENCE_API UTDAOEAbility : public UTDGameplayAbility
{
	GENERATED_BODY()

public:
	UTDAOEAbility();
	~UTDAOEAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
};
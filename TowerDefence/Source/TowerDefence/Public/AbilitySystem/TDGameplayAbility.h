// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TDGameplayAbility.generated.h"

class AProjectileActor;
/**
 *
 */
UCLASS()
class TOWERDEFENCE_API UTDGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UTDGameplayAbility();

	UFUNCTION(BlueprintCallable)
	void SetTarget(AActor* Target);

	UFUNCTION(BlueprintPure)
	AActor* GetTarget();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FScalableFloat Damage;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly);
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	FGameplayEffectSpecHandle GetGameplayEffectSpecHandle();

private:

	UPROPERTY(EditAnywhere, Category = "Target")
	TObjectPtr<AActor> ProjectileTarget;
};
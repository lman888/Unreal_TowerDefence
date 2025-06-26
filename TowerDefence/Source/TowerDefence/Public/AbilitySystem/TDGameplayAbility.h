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

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	void SetTarget(AActor* Target);

	UFUNCTION(BlueprintPure)
	AActor* GetTarget();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FScalableFloat Damage;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AProjectileActor> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly);
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
private:

	UPROPERTY(EditAnywhere, Category = "Target")
	TObjectPtr<AActor> ProjectileTarget;
};
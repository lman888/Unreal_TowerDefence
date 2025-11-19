// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/TDGameplayAbility.h"
#include "TDProjectileAbility.generated.h"

/**
 *
 */
UCLASS()
class TOWERDEFENCE_API UTDProjectileAbility : public UTDGameplayAbility
{
	GENERATED_BODY()

public:

	UTDProjectileAbility();
	~UTDProjectileAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	void SetTarget(AActor* Target);

	UFUNCTION(BlueprintPure)
	AActor* GetTarget();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AProjectileActor> ProjectileClass;

private:
	void SpawnProjectile(FTransform SpawnTransform);

	UPROPERTY(EditAnywhere, Category = "Target")
	TObjectPtr<AActor> ProjectileTarget;

};

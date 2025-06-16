// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TDGameplayAbility.generated.h"

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
	
private:

	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<AActor> ProjectileTarget;
};
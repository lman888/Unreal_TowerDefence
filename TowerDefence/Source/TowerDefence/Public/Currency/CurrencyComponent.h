// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CurrencyComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TOWERDEFENCE_API UCurrencyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCurrencyComponent();

	UFUNCTION(BlueprintCallable)
	void IncreaseGoldAmount(float Gold);

	UFUNCTION(BlueprintCallable)
	void Sell(float SellAmount);

	UFUNCTION(BlueprintPure)
	float GetCostAmount();

	UFUNCTION(BlueprintPure)
	float GetGoldAmount();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GoldAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CostAmount;
};

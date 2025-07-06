// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "TDDeveloperSettings.generated.h"

class AMasterTower;

/**
 * 
 */
UCLASS(config = TowerDefenceSettings, defaultconfig, meta = (DisplayName = "Available Tower Settings"))
class TOWERDEFENCE_API UTDDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UTDDeveloperSettings();

	UPROPERTY(config, EditAnywhere, Category = "Towers")
	TArray<TSubclassOf<AMasterTower>> Towers;
	
	UFUNCTION(BlueprintCallable)
	static const UTDDeveloperSettings* GetMyCustomTDSettings();
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "TDWaveSpawnerSettings.generated.h"

class AEnemyCharacter;

/**
 * 
 */
UCLASS(config = TowerDefenceSettings, defaultconfig, meta = (DisplayName = "Enemy Wave Spawner Settings"))
class TOWERDEFENCE_API UTDWaveSpawnerSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UTDWaveSpawnerSettings();

	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "Waves")
	TArray<TSubclassOf<AEnemyCharacter>> WaveOne;
	
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "Waves")
	TArray<TSubclassOf<AEnemyCharacter>> WaveTwo;

	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "Waves")
	TArray<TSubclassOf<AEnemyCharacter>> WaveThree;

	UFUNCTION(BlueprintCallable)
	static const UTDWaveSpawnerSettings* GetMyCustomWaveSpawner();
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "DeveloperSettings/TDWaveSpawnerSettings.h"

UTDWaveSpawnerSettings::UTDWaveSpawnerSettings()
{
}

const UTDWaveSpawnerSettings* UTDWaveSpawnerSettings::GetMyCustomWaveSpawner()
{
	return GetDefault<UTDWaveSpawnerSettings>();
}

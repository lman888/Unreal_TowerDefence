// Fill out your copyright notice in the Description page of Project Settings.


#include "DeveloperSettings/TDDeveloperSettings.h"

UTDDeveloperSettings::UTDDeveloperSettings()
{
}

const UTDDeveloperSettings* UTDDeveloperSettings::GetMyCustomTDSettings()
{
	return GetDefault<UTDDeveloperSettings>();
}
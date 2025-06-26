// Fill out your copyright notice in the Description page of Project Settings.


#include "TDAssetManager.h"
#include "TDGameplayTags.h"

UTDAssetManager& UTDAssetManager::Get()
{
	check(GEngine->AssetManager);

	UTDAssetManager* TDAssetManager = Cast<UTDAssetManager>(GEngine->AssetManager);
	return *TDAssetManager;
}

void UTDAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FTDGameplayTags::InitializeNativeGameplayTags();
}

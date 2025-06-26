// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "TDAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UTDAssetManager : public UAssetManager
{
	GENERATED_BODY()

	static UTDAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
	
};

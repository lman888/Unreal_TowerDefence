// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerPlacementPlatform.generated.h"

class ATDPlayerPawn;
class UBoxComponent;
class AMasterTower;

UCLASS()
class TOWERDEFENCE_API ATowerPlacementPlatform : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATowerPlacementPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnTowerOnPlatform(AActor* OwningPlayer, TSubclassOf<AMasterTower> Tower);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> Platform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Point")
	TObjectPtr<UBoxComponent> SpawnPoint;

	UPROPERTY(BlueprintReadOnly)
	bool bIsTowerPlaced = false;
};

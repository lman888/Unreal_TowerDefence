// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class AEnemyCharacter;

USTRUCT(Blueprintable)
struct FSpawnParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<AEnemyCharacter>> EnemyAI;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Round = 0;

};

UCLASS()
class TOWERDEFENCE_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FSpawnParams> SpawnParams;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int WaitTime;

private:


};

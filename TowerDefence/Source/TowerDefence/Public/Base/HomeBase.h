// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HomeBase.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class TOWERDEFENCE_API AHomeBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHomeBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Mesh")
	TObjectPtr<UStaticMeshComponent> Base;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box Collision")
	TObjectPtr<UBoxComponent> BoxCollision;

};

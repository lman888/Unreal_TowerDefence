// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HomeBase.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UTDAbilitySystemComponent;
class UTDAttributeSet;
class UGameplayEffect;

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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

private:

	UPROPERTY(EditAnywhere, Category = "AbilitySystem")
	TObjectPtr<UTDAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, Category = "AbilitySystem")
	TObjectPtr<UTDAttributeSet> AttributeSet;
};

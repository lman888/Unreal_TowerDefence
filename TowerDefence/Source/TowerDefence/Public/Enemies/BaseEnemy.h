// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseEnemy.generated.h"

class USphereComponent;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class TOWERDEFENCE_API ABaseEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere, Category = "AbilitySystem")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(EditAnywhere, Category = "AbilitySystem")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category = "Enemy Body")
	TObjectPtr<UStaticMeshComponent> EnemyBody;

	UPROPERTY(EditAnywhere, Category = "Enemy Hit Box")
	TObjectPtr<USphereComponent> SphereComponent;
};
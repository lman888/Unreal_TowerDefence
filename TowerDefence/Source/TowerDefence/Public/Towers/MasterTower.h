// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "Interaction/TowerInterface.h"
#include "MasterTower.generated.h"

class UGameplayAbility;
class UTDAbilitySystemComponent;
class UGameplayEffect;
class UBoxComponent;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class TOWERDEFENCE_API AMasterTower : public AActor, public IAbilitySystemInterface, public ITowerInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMasterTower();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	UAttributeSet* GetAttributeSet() const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Targeting();

	UFUNCTION(BlueprintPure)
	AActor* GetTargetedEnemy();

protected:
	virtual FTransform GetProjectileSpawnLocation() override;

private:

	//This is getting called in BeginPlay. I am not sure why it says "No blueprint usages".
	UFUNCTION(BlueprintCallable)
	void InitializeAttributes() const;

	void AddTowerAbility(TSubclassOf<UGameplayAbility>& Ability) const;
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Mesh")
	TObjectPtr<UStaticMeshComponent> TowerBase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Mesh")
	TObjectPtr<UStaticMeshComponent> TowerBody;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Mesh")
	TObjectPtr<UStaticMeshComponent> TowerHead;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Mesh")
	TObjectPtr<UStaticMeshComponent> ProjectileTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float ClosestDistance;
	
	UPROPERTY(EditAnywhere, Category = "AbilitySystem")
	TObjectPtr<UTDAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(EditAnywhere, Category = "AbilitySystem")
	TObjectPtr<UAttributeSet> AttributeSet;
	UPROPERTY(EditAnywhere, Category = "Tower Hit Box")
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(BlueprintReadWrite, Category = "Targeting")
	TArray<AActor*> EnemiesInRadius;

	UPROPERTY(BlueprintReadOnly, Category = "Targeting")
	TObjectPtr<AActor> TargetedEnemy;
	
	UPROPERTY(EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> TowerAttributes;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TSubclassOf<UGameplayAbility> TowerAbilities;
};
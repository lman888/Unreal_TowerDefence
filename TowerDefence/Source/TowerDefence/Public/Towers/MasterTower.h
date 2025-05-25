// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "MasterTower.generated.h"

class UGameplayEffect;
class UBoxComponent;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class TOWERDEFENCE_API AMasterTower : public AActor, public IAbilitySystemInterface
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

private:

	UFUNCTION(BlueprintCallable)
	void InitializeAttributes() const;
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Base")
	TObjectPtr<UStaticMeshComponent> TowerBase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Body")
	TObjectPtr<UStaticMeshComponent> TowerBody;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Head")
	TObjectPtr<UStaticMeshComponent> TowerHead;
	
	UPROPERTY(EditAnywhere, Category = "AbilitySystem")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(EditAnywhere, Category = "AbilitySystem")
	TObjectPtr<UAttributeSet> AttributeSet;
	UPROPERTY(EditAnywhere, Category = "Tower Hit Box")
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> TowerAttributes;
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/TDCombatInterface.h"
#include "HomeBase.generated.h"

class UWidgetComponent;
class UBoxComponent;
class UStaticMeshComponent;
class UTDAbilitySystemComponent;
class UTDAttributeSet;
class UGameplayEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHomeBaseAttributeChanged, float, NewValue);

UCLASS()
class TOWERDEFENCE_API AHomeBase : public AActor, public ITDCombatInterface
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

	UFUNCTION()
	virtual void HandleDeath() override;

	UFUNCTION()
	UTDAbilitySystemComponent* GetAbilitySystemComponent();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Mesh")
	TObjectPtr<UStaticMeshComponent> Base;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box Collision")
	TObjectPtr<UBoxComponent> BoxCollision;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(BlueprintAssignable)
	FOnHomeBaseAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnHomeBaseAttributeChanged OnMaxHealthChanged;
	
private:

	void ApplyEffectSpec(const TSubclassOf<UGameplayEffect>& GameplayEffect, float Level) const;
	
	UPROPERTY(EditAnywhere, Category = "AbilitySystem")
	TObjectPtr<UTDAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, Category = "AbilitySystem")
	TObjectPtr<UTDAttributeSet> AttributeSet;

};

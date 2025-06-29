// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/TDCombatInterface.h"
#include "EnemyCharacter.generated.h"

class UWidgetComponent;
class UNiagaraSystem;
class UGameplayEffect;
class UTDAbilitySystemComponent;
class UTDAttributeSet;
class USphereComponent;
class ITDCombatInterface;
class USoundWave;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChanged, float, NewValue);

UCLASS()
class TOWERDEFENCE_API AEnemyCharacter : public ACharacter, public ITDCombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	UTDAbilitySystemComponent* GetAbilitySystemComponent() const;

	void InitializeAttributes() const;

	void ApplyEffectSpec(const TSubclassOf<UGameplayEffect>& GameplayEffect, float Level) const;

	UFUNCTION(BlueprintPure)
	UTDAttributeSet* GetAttributeSet() const;

	UFUNCTION()
	virtual void HandleDeath() override;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnMaxHealthChanged;
	
protected:
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

private:
	UPROPERTY(EditAnywhere, Category = "AbilitySystem")
	TObjectPtr<UTDAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, Category = "AbilitySystem")
	TObjectPtr<UTDAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraSystem* DeathEffect;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* DeathSound;
};

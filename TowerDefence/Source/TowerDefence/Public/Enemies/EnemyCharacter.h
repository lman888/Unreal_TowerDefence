// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class UGameplayEffect;
class UTDAbilitySystemComponent;
class UTDAttributeSet;
class USphereComponent;

UCLASS()
class TOWERDEFENCE_API AEnemyCharacter : public ACharacter
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

	UFUNCTION(BlueprintPure)
	UTDAttributeSet* GetAttributeSet() const;

protected:
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultAttributes;

private:
	UPROPERTY(EditAnywhere, Category = "AbilitySystem")
	TObjectPtr<UTDAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, Category = "AbilitySystem")
	TObjectPtr<UTDAttributeSet> AttributeSet;
};

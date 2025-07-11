// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "TDAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UTDAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UTDAttributeSet();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Main Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, Health);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Main Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, MaxHealth);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackSpeed, Category = "Main Attributes")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, AttackSpeed);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_RangedArmour, Category = "Main Attributes")
	FGameplayAttributeData RangedArmour;
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, RangedArmour);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MovementSpeed, Category = "Main Attributes")
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, MovementSpeed);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackRange, Category = "Main Attributes")
	FGameplayAttributeData AttackRange;
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, AttackRange);

	/*
	 * Meta Attributes
	 */

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UTDAttributeSet, IncomingDamage);
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& oldHealth) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& oldMaxHealth) const;
	UFUNCTION()
	void OnRep_AttackSpeed(const FGameplayAttributeData& oldAttackSpeed) const;
	UFUNCTION()
	void OnRep_RangedArmour(const FGameplayAttributeData& oldRangedArmour) const;
	UFUNCTION()
	void OnRep_MovementSpeed(const FGameplayAttributeData& oldMovementSpeed) const;
	UFUNCTION()
	void OnRep_AttackRange(const FGameplayAttributeData& oldAttackRange) const;
};

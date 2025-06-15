// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TDAbilitySystemComponent.h"

void UTDAbilitySystemComponent::AddCharacterAbility(TSubclassOf<UGameplayAbility>& Ability)
{
	const FGameplayAbilitySpec chosenAbility = FGameplayAbilitySpec(*Ability, 1);
	GiveAbility(chosenAbility);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TDAbilitySystemComponent.h"

#include "AbilitySystem/TDGameplayAbility.h"
#include "Towers/MasterTower.h"

void UTDAbilitySystemComponent::AddCharacterAbility(TSubclassOf<UGameplayAbility>& Ability)
{
	AMasterTower* Owner = Cast<AMasterTower>(GetAvatarActor());
	if (Owner == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Owner is invalid!"));
		return;
	}
	
	const FGameplayAbilitySpec ChosenAbility = FGameplayAbilitySpec(*Ability, Owner->GetTowerLevel());
	GiveAbility(ChosenAbility);
}

void UTDAbilitySystemComponent::UpgradeAbility(TSubclassOf<UGameplayAbility>& Ability) const
{
	AMasterTower* Owner = Cast<AMasterTower>(GetAvatarActor());
	if (Owner == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Owner is invalid!"));
		return;
	}

	FGameplayAbilitySpec* Spec = FindAbilitySpecFromClass(*Ability);
	if (Spec)
	{
		Spec->Level = Owner->GetTowerLevel();		
	}
}
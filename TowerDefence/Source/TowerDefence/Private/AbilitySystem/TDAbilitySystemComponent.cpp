// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TDAbilitySystemComponent.h"

#include "AbilitySystem/TDGameplayAbility.h"
#include "Towers/MasterTower.h"
#include "Enemies/EnemyCharacter.h"

void UTDAbilitySystemComponent::AddCharacterAbility(TSubclassOf<UGameplayAbility> Ability)
{
	//Do this a better way in future

	AMasterTower* TowerAbility = Cast<AMasterTower>(GetAvatarActor());
	if (TowerAbility)
	{
		const FGameplayAbilitySpec ChosenAbility = FGameplayAbilitySpec(*Ability, TowerAbility->GetTowerLevel());
		GiveAbility(ChosenAbility);
		return;
	}

	AEnemyCharacter* UFOAbility = Cast<AEnemyCharacter>(GetAvatarActor());
	if (UFOAbility)
	{
		const FGameplayAbilitySpec ChosenAbility = FGameplayAbilitySpec(*Ability, 1);
		//Dont forget about the AI, we use this as well to give them the ability
		GiveAbility(ChosenAbility);
		return;
	}
	
	UE_LOG(LogTemp, Display, TEXT("Owner is invalid!"));
}

void UTDAbilitySystemComponent::UpgradeAbility(TSubclassOf<UGameplayAbility> Ability) const
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
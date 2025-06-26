// Fill out your copyright notice in the Description page of Project Settings.


#include "TDGameplayTags.h"
#include "GameplayTagsManager.h"

FTDGameplayTags FTDGameplayTags::GameplayTags;

void FTDGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.Health = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Health"), FString("Health of the Actor"));
	GameplayTags.AttackSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.AttackSpeed"), FString("Attack Speed of the Actor"));
	GameplayTags.RangedArmour = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.RangedArmour"), FString("Ranged Armour of the Actor"));
	GameplayTags.MovementSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.MovementSpeed"), FString("Speed of the Actor"));
	GameplayTags.AttackRange = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.AttackRange"), FString("Attack Range of the Actor"));

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), FString("Incoming Damage to an Actor"));
}
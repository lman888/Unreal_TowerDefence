// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TDAttributeSet.h"

#include "Net/UnrealNetwork.h"

UTDAttributeSet::UTDAttributeSet()
{
}

void UTDAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, RangedArmour, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, AttackRange, COND_None, REPNOTIFY_Always);
}

void UTDAttributeSet::OnRep_Health(const FGameplayAttributeData& oldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, Health, oldHealth);
}

void UTDAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& oldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, MaxHealth, oldMaxHealth);
}

void UTDAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& oldAttackSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, AttackSpeed, oldAttackSpeed);
}

void UTDAttributeSet::OnRep_RangedArmour(const FGameplayAttributeData& oldRangedArmour) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, RangedArmour, oldRangedArmour);
}

void UTDAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& oldMovementSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, MovementSpeed, oldMovementSpeed);
}

void UTDAttributeSet::OnRep_AttackRange(const FGameplayAttributeData& oldAttackRange) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, AttackRange, oldAttackRange);
}
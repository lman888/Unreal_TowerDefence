// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TDAttributeSet.h"

#include "Net/UnrealNetwork.h"

UTDAttributeSet::UTDAttributeSet()
{
	InitMaxHealth(100.0f);
	InitHealth(MaxHealth.GetCurrentValue());
	InitAttackSpeed(15.0f);
	InitMeleeArmour(10.0f);
	InitRangedArmour(10.0f);
	InitMovementSpeed(0.0f);
}

void UTDAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, MeleeArmour, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, RangedArmour, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
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

void UTDAttributeSet::OnRep_MeleeArmour(const FGameplayAttributeData& oldMeleeArmour) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, MeleeArmour, oldMeleeArmour);
}

void UTDAttributeSet::OnRep_RangedArmour(const FGameplayAttributeData& oldRangedArmour) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, RangedArmour, oldRangedArmour);
}

void UTDAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& oldMovementSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDAttributeSet, MovementSpeed, oldMovementSpeed);
}
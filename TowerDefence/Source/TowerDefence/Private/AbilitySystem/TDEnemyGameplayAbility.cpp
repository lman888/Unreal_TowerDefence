// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TDEnemyGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TDGameplayTags.h"
#include "AbilitySystem/TDAbilitySystemComponent.h"
#include "Base/HomeBase.h"
#include "Enemies/EnemyCharacter.h"

void UTDEnemyGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CommitAbility(Handle, ActorInfo, ActivationInfo);

	if (!HasAuthority(&ActivationInfo))
	{
		return;
	}

	AEnemyCharacter* Owner = Cast<AEnemyCharacter>(GetAvatarActorFromActorInfo());
	if (Owner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owning Actor is invade for %s"), *GetName());
		return;
	}

	const UTDAbilitySystemComponent* SourceASC = Owner->GetAbilitySystemComponent();
	if (SourceASC == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owning Actor has an Invalid Ability System Component in %s"), *GetName());
		return;
	}
	
	UTDAbilitySystemComponent* TargetBaseASC = Owner->GetTarget()->GetAbilitySystemComponent();
	if (TargetBaseASC == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Base has an Invalid Ability System Component in %s"), *GetName());
		return;
	}
	
	const FGameplayEffectSpecHandle DamageSpecHandle = SourceASC->MakeOutgoingSpec(DamageEffect, GetAbilityLevel(), SourceASC->MakeEffectContext());
	const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	FTDGameplayTags GameplayTags = FTDGameplayTags::Get();
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, GameplayTags.Damage, ScaledDamage);
	
	Owner->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), TargetBaseASC);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TDGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TDGameplayTags.h"
#include "AbilitySystem/TDAbilitySystemComponent.h"
#include "Towers/MasterTower.h"

UTDGameplayAbility::UTDGameplayAbility()
{

}

FGameplayEffectSpecHandle UTDGameplayAbility::GetGameplayEffectSpecHandle()
{
	//Give Projectile a Gameplay Effect Spec for causing Damage
	const UTDAbilitySystemComponent* SourceASC = Cast<UTDAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo()));
	if (IsValid(SourceASC) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Source Ability System Component is invalid inside %s."), *GetName());
		return FGameplayEffectSpecHandle();
	}

	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
	if (SpecHandle.IsValid() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spec Handle is invalid inside %s."), *GetName());
		return FGameplayEffectSpecHandle();
	}

	FTDGameplayTags GameplayTags = FTDGameplayTags::Get();
	const float ScaledDamage = Damage.GetValueAtLevel(SpecHandle.Data.Get()->GetLevel());

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Basic Turret Damage: %f"), ScaledDamage));
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, ScaledDamage);

	return SpecHandle;
}
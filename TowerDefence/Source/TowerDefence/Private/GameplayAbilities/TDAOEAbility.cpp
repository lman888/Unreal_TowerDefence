// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/TDAOEAbility.h"
#include <Interaction/TowerInterface.h>
#include <Kismet/GameplayStatics.h>
#include "Enemies/EnemyCharacter.h"
#include <AbilitySystemBlueprintLibrary.h>
#include "AbilitySystem/TDAbilitySystemComponent.h"
#include <Towers/MasterTower.h>

UTDAOEAbility::UTDAOEAbility()
{
}

UTDAOEAbility::~UTDAOEAbility()
{
}

void UTDAOEAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CommitAbility(Handle, ActorInfo, ActivationInfo);

	if (HasAuthority(&ActivationInfo) == false)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ITowerInterface* towerInterface = Cast<ITowerInterface>(GetAvatarActorFromActorInfo());
	if (towerInterface == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Tower Interface in the ability: %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AMasterTower* Owner = Cast<AMasterTower>(GetAvatarActorFromActorInfo());
	if (Owner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("The tower that casted this ability [%s] is apparently invalid, not good."), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (Owner->GetEnemiesInRange().IsEmpty() == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("The tower has no targets in range when this ability was activated [%s]."), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	//Depending on tower level, select a certain amount of enemies and freeze them.
	//Definetly a better way of doing this, will look at a bit later.
	for (size_t i = 0; i < Owner->GetEnemiesInRange().Num(); i++)
	{
		if (i > GetAbilityLevel())
		{
			break;
		}

		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner->GetEnemiesInRange()[i]))
		{
			//Apply the effect to those enemies.
			TargetASC->ApplyGameplayEffectSpecToSelf(*GetGameplayEffectSpecHandle().Data.Get());
		}
	}

	//End Ability.
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/TDAOEAbility.h"
#include <Interaction/TowerInterface.h>
#include <Kismet/GameplayStatics.h>
#include "Enemies/EnemyCharacter.h"
#include <AbilitySystemBlueprintLibrary.h>
#include "AbilitySystem/TDAbilitySystemComponent.h"

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

	//Find enemies in range.
	TArray<AActor*> EnemiesInRange;
	TArray<AActor*> EnemiesInLevel;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), EnemiesInLevel);

	for (AActor* Enemy : EnemiesInLevel)
	{
		float Distance = FVector::Distance(Enemy->GetActorLocation(), GetOwningActorFromActorInfo()->GetActorLocation());

		if (Distance <= TowerRange)
		{
			EnemiesInRange.Add(Enemy);
		}
	}

	//Depending on tower level, select a certain amount of enemies.
	for (size_t i = 0; i < GetAbilityLevel(); i++)
	{
		int32 RandomIndex = FMath::RandRange(0, EnemiesInRange.Num() - 1);

		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(EnemiesInRange[RandomIndex]))
		{
			//Apply the effect to those enemies.
			TargetASC->ApplyGameplayEffectSpecToSelf(*GetGameplayEffectSpecHandle().Data.Get());
		}
	}

	//End Ability.
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}
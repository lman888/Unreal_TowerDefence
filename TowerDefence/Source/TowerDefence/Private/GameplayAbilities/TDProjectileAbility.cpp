// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/TDProjectileAbility.h"
#include <Interaction/TowerInterface.h>
#include "Projectiles/ProjectileActor.h"

UTDProjectileAbility::UTDProjectileAbility()
{
}

UTDProjectileAbility::~UTDProjectileAbility()
{
}

void UTDProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CommitAbility(Handle, ActorInfo, ActivationInfo);

	if (!HasAuthority(&ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ITowerInterface* towerInterface = Cast<ITowerInterface>(GetAvatarActorFromActorInfo());
	if (towerInterface)
	{
		FTransform SpawnTransform = towerInterface->GetProjectileSpawnLocation();

		FRotator newRotation = SpawnTransform.Rotator();

		newRotation.Yaw += 90.0f;

		SpawnTransform.SetRotation(newRotation.Quaternion());

		if (ProjectileClass && DamageEffectClass == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Damage Effect or Projectile Class have been assigned to %s"), *GetName());
			return;
		}

		if (ProjectileClass)
		{
			SpawnProjectile(SpawnTransform);
		}

		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UTDProjectileAbility::SpawnProjectile(FTransform SpawnTransform)
{
	AProjectileActor* Projectile = GetWorld()->SpawnActorDeferred<AProjectileActor>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (Projectile == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile is Invalid inside %s"), *GetName());
		return;
	}

	Projectile->DamageEffectSpecHandle = GetGameplayEffectSpecHandle();

	Projectile->FinishSpawning(SpawnTransform);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TDGameplayAbility.h"

#include "Interaction/TowerInterface.h"
#include "Projectiles/ProjectileActor.h"

UTDGameplayAbility::UTDGameplayAbility()
{

}

void UTDGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!HasAuthority(&ActivationInfo))
	{
		return;
	}

	ITowerInterface* towerInterface = Cast<ITowerInterface>(GetAvatarActorFromActorInfo());
	if (towerInterface)
	{
		FTransform SpawnTransform = towerInterface->GetProjectileSpawnLocation();

		FRotator newRotation = SpawnTransform.Rotator();

		newRotation.Yaw += 90.0f;

		SpawnTransform.SetRotation(newRotation.Quaternion());
		
		AProjectileActor* Projectile = GetWorld()->SpawnActorDeferred<AProjectileActor>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(),
											     Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		if (Projectile == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Projectile is Invalid inside %s"), *GetName());
			return;
		}
		
		//Give Projectile a Gameplay Effect Spec for causing Damage
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}

void UTDGameplayAbility::SetTarget(AActor* Target)
{
	ProjectileTarget = Target;
}

AActor* UTDGameplayAbility::GetTarget()
{
	return ProjectileTarget;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TDGameplayAbility.h"

#include "Interaction/TowerInterface.h"
#include "Projectiles/ProjectileActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "TDGameplayTags.h"
#include "AbilitySystem/TDAbilitySystemComponent.h"

UTDGameplayAbility::UTDGameplayAbility()
{

}

void UTDGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CommitAbility(Handle, ActorInfo, ActivationInfo);
	
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
		const UTDAbilitySystemComponent* SourceASC = Cast<UTDAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo()));
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());

		FTDGameplayTags GameplayTags = FTDGameplayTags::Get();
		const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Basic Turret Damage: %f"), ScaledDamage));
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, ScaledDamage);
		
		Projectile->DamageEffectSpecHandle = SpecHandle;
		
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

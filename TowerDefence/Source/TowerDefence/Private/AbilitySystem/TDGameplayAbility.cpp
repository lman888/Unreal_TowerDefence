// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TDGameplayAbility.h"

UTDGameplayAbility::UTDGameplayAbility()
{
	ProjectileBody = CreateDefaultSubobject<UStaticMeshComponent>("Projectile Body");
}

void UTDGameplayAbility::SetTarget(AActor* Target)
{
	ProjectileTarget = Target;
}

AActor* UTDGameplayAbility::GetTarget()
{
	return ProjectileTarget;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "ProjectileActor.generated.h"

struct FGameplayEffectSpecHandle;
class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraSystem;

UCLASS()
class TOWERDEFENCE_API AProjectileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void ApplyDamage() const;
	
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> ProjectileBody;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	TObjectPtr<USphereComponent> SphereCollision;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "Effects")
	FGameplayEffectSpecHandle DamageEffectSpecHandle;
};

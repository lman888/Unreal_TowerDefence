// Fill out your copyright notice in the Description page of Project Settings.

#include "MasterTower.h"

#include "AbilitySystem/TDAbilitySystemComponent.h"
#include "AbilitySystem/TDAttributeSet.h"
#include "Components/BoxComponent.h"

// Sets default values
AMasterTower::AMasterTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AbilitySystemComponent = CreateDefaultSubobject<UTDAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UTDAttributeSet>("AttributeSet");

	TowerBase = CreateDefaultSubobject<UStaticMeshComponent>("Tower Base");
	SetRootComponent(TowerBase);
	TowerHead = CreateDefaultSubobject<UStaticMeshComponent>("Tower Head");
	TowerHead->SetupAttachment(TowerBase);
	TowerBody = CreateDefaultSubobject<UStaticMeshComponent>("Tower Body");
	TowerBody->SetupAttachment(TowerBase);
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box Component");
	BoxComponent->SetupAttachment(TowerBase);
}

UAbilitySystemComponent* AMasterTower::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AMasterTower::GetAttributeSet() const
{
	return AttributeSet;
}

// Called when the game starts or when spawned
void AMasterTower::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMasterTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
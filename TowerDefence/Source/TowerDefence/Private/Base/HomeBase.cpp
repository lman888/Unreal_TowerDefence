// Fill out your copyright notice in the Description page of Project Settings.


#include "Base/HomeBase.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystem/TDAbilitySystemComponent.h"
#include "AbilitySystem/TDAttributeSet.h"

// Sets default values
AHomeBase::AHomeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Base = CreateDefaultSubobject<UStaticMeshComponent>("Base Mesh");
	SetRootComponent(Base);
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("Box Collision");
	BoxCollision->SetupAttachment(Base);

	AbilitySystemComponent = CreateDefaultSubobject<UTDAbilitySystemComponent>("AbilitySystem Component");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UTDAttributeSet>("AttributeSet");

}

// Called when the game starts or when spawned
void AHomeBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHomeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


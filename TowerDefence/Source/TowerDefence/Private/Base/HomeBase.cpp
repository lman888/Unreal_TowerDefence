// Fill out your copyright notice in the Description page of Project Settings.


#include "Base/HomeBase.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystem/TDAbilitySystemComponent.h"
#include "AbilitySystem/TDAttributeSet.h"
#include "Components/WidgetComponent.h"

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

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("Home Health Bar");
	HealthBar->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AHomeBase::BeginPlay()
{
	Super::BeginPlay();

	ApplyEffectSpec(PrimaryAttributes, 1);
	ApplyEffectSpec(SecondaryAttributes, 1);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
	{
		OnHealthChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
	{
		OnMaxHealthChanged.Broadcast(Data.NewValue);
	});

	OnHealthChanged.Broadcast(AttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AttributeSet->GetMaxHealth());
}

// Called every frame
void AHomeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHomeBase::HandleDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("Home Base has died"));
}

UTDAbilitySystemComponent* AHomeBase::GetAbilitySystemComponent()
{
	return AbilitySystemComponent;
}

void AHomeBase::ApplyEffectSpec(const TSubclassOf<UGameplayEffect>& GameplayEffect, float Level) const
{
	check(IsValid(AbilitySystemComponent));
	check(GameplayEffect);
	
	const FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, Level, AbilitySystemComponent->MakeEffectContext());
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}

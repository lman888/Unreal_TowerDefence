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
	ProjectileTransform = CreateDefaultSubobject<UStaticMeshComponent>("Projectile Spawn");
	ProjectileTransform->SetupAttachment(TowerHead);

    if (AttributeSet == nullptr)
    {
	    UE_LOG(LogTemp, Warning, TEXT("Attribute Set Null for %s"), *GetName());
    	return;
    }

    if (!HasAuthority())
    {
    	return;
    }

	TargetedEnemy = nullptr;
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
	
	AddTowerAbility(TowerAbilities);
}

// Called every frame
void AMasterTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMasterTower::Targeting()
{
	//Loop through all enemies inside the attack radius
	for (AActor* Enemy : EnemiesInRadius)
	{
		FVector enemyLocation = Enemy->GetActorLocation();
		FVector turretLocation = GetActorLocation();

		UE::Math::TVector<double> vectorLength = enemyLocation - turretLocation;

		if (ClosestDistance <= vectorLength.Length())
		{
			TargetedEnemy = Enemy;
		}
	}
}

AActor* AMasterTower::GetTargetedEnemy()
{
	return TargetedEnemy;
}

FTransform AMasterTower::GetProjectileSpawnLocation()
{
	return ProjectileTransform->GetComponentTransform();
}

void AMasterTower::InitializeAttributes() const
{
	if (TowerAttributes == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Please assign an Attribute Set to Tower Attributes!"));
		return;
	}

	const FGameplayEffectSpecHandle TowerAttribute = AbilitySystemComponent->MakeOutgoingSpec(TowerAttributes, 1, AbilitySystemComponent->MakeEffectContext());
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*TowerAttribute.Data.Get());
}

void AMasterTower::AddTowerAbility(TSubclassOf<UGameplayAbility>& Ability) const
{
	if (Ability == nullptr)
	{
		return;
	}
	
	AbilitySystemComponent->AddCharacterAbility(Ability);
}
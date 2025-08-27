// Fill out your copyright notice in the Description page of Project Settings.

#include "MasterTower.h"

#include "AbilitySystem/TDAbilitySystemComponent.h"
#include "AbilitySystem/TDAttributeSet.h"
#include "AbilitySystem/TDGameplayAbility.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AMasterTower::AMasterTower()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

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

	TowerLevel = 1;
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

	AddTowerAbility(TowerAbility);

	UpdateTowerUpgradeWidgetInformation();
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
int AMasterTower::GetTowerLevel() const
{
	return TowerLevel;
}

FTransform AMasterTower::GetProjectileSpawnLocation()
{
	return ProjectileTransform->GetComponentTransform();
}

void AMasterTower::UpgradeTower_Implementation()
{
	if (HasAuthority())
	{
		//Change out Tower Mesh here
		SetTowerHeadMaterial();

		if (TowerLevel == MaxTowerLevel)
		{
			return;
		}

		TowerLevel++;

		AbilitySystemComponent->UpgradeAbility(TowerAbility);

		UpdateTowerUpgradeWidgetInformation();
	}
}

void AMasterTower::InitializeAttributes() const
{
	if (TowerAttributes == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Please assign an Attribute Set to Tower Attributes!"));
		return;
	}

	const FGameplayEffectSpecHandle TowerAttribute = AbilitySystemComponent->MakeOutgoingSpec(TowerAttributes, GetTowerLevel(), AbilitySystemComponent->MakeEffectContext());
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

void AMasterTower::UpdateTowerUpgradeWidgetInformation_Implementation()
{
	if (HasAuthority())
	{
		UTDGameplayAbility* Ability = Cast<UTDGameplayAbility>(TowerAbility.GetDefaultObject());
		if (Ability == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Ability is no valid!"));
			return;
		}

		FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromClass(TowerAbility);
		if (Spec == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Spec is no valid!"));
			return;
		}

		TowerInfo.TowerDamage = Ability->Damage.GetValueAtLevel(Spec->Level);
	}
}

void AMasterTower::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMasterTower, TowerInfo);
	DOREPLIFETIME(AMasterTower, TowerAbility);
	DOREPLIFETIME(AMasterTower, AbilitySystemComponent);
}

void AMasterTower::SetTowerHeadMaterial()
{
	int32 TowerMeshIndex = TowerLevel - 1;

	if (TowerUpgradeMaterial.IsValidIndex(TowerMeshIndex))
	{
		TowerHead->SetMaterial(0, TowerUpgradeMaterial[TowerMeshIndex]);
	}
}
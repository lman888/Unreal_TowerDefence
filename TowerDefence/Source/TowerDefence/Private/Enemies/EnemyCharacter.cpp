// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyCharacter.h"

#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/TDAbilitySystemComponent.h"
#include "AbilitySystem/TDAttributeSet.h"
#include "Base/HomeBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UTDAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UTDAttributeSet>("AttributeSet");

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>("Health Bar Component");
	HealthBarWidget->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeAttributes();

	AbilitySystemComponent->AddCharacterAbility(DamageAbility);

	GetCharacterMovement()->MaxWalkSpeed =  AttributeSet->GetMovementSpeed();

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
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UTDAbilitySystemComponent* AEnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

AActor* AEnemyCharacter::GetTarget() const
{
	return Target;
}

void AEnemyCharacter::InitializeAttributes() const
{
	ApplyEffectSpec(PrimaryAttributes, 1.0f);
	ApplyEffectSpec(SecondaryAttributes, 1.0f);
}

void AEnemyCharacter::ApplyEffectSpec(const TSubclassOf<UGameplayEffect>& GameplayEffect, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffect);
	
	const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle EffectSpec = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffect, 1, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data.Get(), GetAbilitySystemComponent());
}

UTDAttributeSet* AEnemyCharacter::GetAttributeSet() const
{
	return AttributeSet;
}

void AEnemyCharacter::HandleDeath()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Object: %s has died"), *GetName()));

	if (DeathEffect == nullptr || DeathSound == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Death Sound or Death Effect has not been set in %s"), *GetName());
		return;
	}
	
	UGameplayStatics::PlaySound2D(GetWorld(), DeathSound);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathEffect, GetActorLocation(), GetActorRotation());
	
	Destroy();
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "Interaction/TowerInterface.h"
#include "MasterTower.generated.h"

class UGameplayAbility;
class UTDAbilitySystemComponent;
class UGameplayEffect;
class UBoxComponent;
class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageChanged, int, NewDamage);

USTRUCT(BlueprintType)
struct FTowerInformation
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString TowerName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString TowerDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int TowerDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TowerCost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* TowerImage;
};

UCLASS()
class TOWERDEFENCE_API AMasterTower : public AActor, public IAbilitySystemInterface, public ITowerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMasterTower();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	UAttributeSet* GetAttributeSet() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Targeting();

	UFUNCTION(BlueprintPure)
	AActor* GetTargetedEnemy();

	UFUNCTION(BlueprintPure)
	int GetTowerLevel() const;

	UFUNCTION(BlueprintPure)
	TArray<AActor*> GetEnemiesInRange();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Textures")
	TArray<UMaterialInterface*> TowerUpgradeMaterial;

	virtual FTransform GetProjectileSpawnLocation() override;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void UpgradeTower();

	UFUNCTION()
	void LocalSetTowerHeadMesh();

private:

	//This is getting called in BeginPlay. I am not sure why it says "No blueprint usages".
	UFUNCTION(BlueprintCallable)
	void InitializeAttributes() const;

	void HandleLocalTowerUpgrade();

	UFUNCTION(Server, Reliable)
	void AddTowerAbility(TSubclassOf<UGameplayAbility> Ability);

	UFUNCTION(Client, Reliable)
	void ClientAddTowerAbility(TSubclassOf<UGameplayAbility> Ability);

	UFUNCTION(Client, Reliable)
	void ClientSetTowerHeadMaterial(UMaterialInterface* NewMaterial);

	UFUNCTION(Server, Reliable)
	void UpdateTowerUpgradeWidgetInformation();

	UFUNCTION()
	void LocalUpdateTowerUpgradeWidgetInformation();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = OnRep_TowerLevelUp)
	UMaterialInterface* TowerMaterial = nullptr;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Mesh")
	TObjectPtr<UStaticMeshComponent> TowerBase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Mesh")
	TObjectPtr<UStaticMeshComponent> TowerBody;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Mesh")
	TObjectPtr<UStaticMeshComponent> TowerHead;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Mesh")
	TObjectPtr<UStaticMeshComponent> ProjectileTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float ClosestDistance;

	UPROPERTY(Replicated, EditAnywhere, Category = "AbilitySystem")
	TObjectPtr<UTDAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, Category = "AbilitySystem")
	TObjectPtr<UAttributeSet> AttributeSet;
	UPROPERTY(EditAnywhere, Category = "Tower Hit Box")
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(BlueprintReadWrite, Category = "Targeting")
	TArray<AActor*> EnemiesInRadius;

	UPROPERTY(BlueprintReadOnly, Category = "Targeting")
	TObjectPtr<AActor> TargetedEnemy;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> TowerAttributes;

	UPROPERTY(ReplicatedUsing = OnRep_AddTowerAbility, EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<UGameplayAbility> TowerAbility;

	UPROPERTY(ReplicatedUsing = OnRep_TowerUpdateInfo, EditDefaultsOnly, BlueprintReadOnly)
	FTowerInformation TowerInfo;

	UPROPERTY(ReplicatedUsing = OnRep_TowerLevelUp, EditDefaultsOnly, Category = "Level")
	int TowerLevel;

	UPROPERTY(EditDefaultsOnly, Category = "Level")
	int MaxTowerLevel;

	UPROPERTY(BlueprintAssignable)
	FOnDamageChanged OnDamageChanged;

	UFUNCTION(BlueprintPure)
	FTowerInformation GetTowerInfo();

	UFUNCTION()
	void OnRep_TowerLevelUp();

	UFUNCTION()
	void OnRep_TowerUpdateInfo();

	UFUNCTION()
	void OnRep_AddTowerAbility();

	UFUNCTION(Server, Reliable)
	void ServerSetTowerHeadMaterial();
};
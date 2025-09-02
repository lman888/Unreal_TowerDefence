// Fill out your copyright notice in the Description page of Project Settings.


#include "Platforms/TowerPlacementPlatform.h"

#include "IAutomationControllerManager.h"
#include "Components/BoxComponent.h"
#include "Towers/MasterTower.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATowerPlacementPlatform::ATowerPlacementPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	Platform = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Mesh"));
	SetRootComponent(Platform);

	SpawnPoint = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawn Point"));
	SpawnPoint->SetupAttachment(Platform);
}

// Called when the game starts or when spawned
void ATowerPlacementPlatform::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATowerPlacementPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ATowerPlacementPlatform::SpawnTowerOnPlatform(AActor* OwningPlayer, TSubclassOf<AMasterTower> Tower)
{
	if (HasAuthority())
	{
		if (!IsValid(Tower))
		{
			UE_LOG(LogTemp, Display, TEXT("Tower Object is not Valid."));
			return;
		}

		const AMasterTower* DefaultTower = Tower->GetDefaultObject<AMasterTower>();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwningPlayer;

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SpawnPoint->GetComponentLocation());
		SpawnTransform.SetRotation(DefaultTower->GetActorQuat());
		SpawnTransform.SetScale3D(DefaultTower->GetActorScale3D());

		if (HasAuthority())
		{
			GetWorld()->SpawnActor<AMasterTower>(Tower, SpawnTransform, SpawnParams);
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/TDPlayerPawn.h"

// Sets default values
ATDPlayerPawn::ATDPlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CoinAmount = 0;

}

int ATDPlayerPawn::GetCointAmount()
{
	return CoinAmount;
}

void ATDPlayerPawn::AddToCointAmount(int AmountToAdd)
{
	CoinAmount += AmountToAdd;
}

// Called when the game starts or when spawned
void ATDPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATDPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATDPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


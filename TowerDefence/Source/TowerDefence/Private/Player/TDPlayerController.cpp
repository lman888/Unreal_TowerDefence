// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TDPlayerController.h"

#include "Platforms/TowerPlacementPlatform.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemTypes.h"
#include "Interfaces/OnlineIdentityInterface.h"


ATDPlayerController::ATDPlayerController()
{
	bReplicates = true;
}

void ATDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	Login();
}

void ATDPlayerController::Login()
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	if (Subsystem == nullptr)
	{
		return;
	}

	IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
	if (Identity == nullptr)
	{
		return;
	}

	FUniqueNetIdPtr NetId = Identity->GetUniquePlayerId(0);
	if (NetId != nullptr && Identity->GetLoginStatus(0) == ELoginStatus::LoggedIn)
	{
		return;
	}

	LoginDelegateHandle = Identity->AddOnLoginCompleteDelegate_Handle(0, FOnLoginCompleteDelegate::CreateUObject(this, &ATDPlayerController::HandleLoginCompleted));

	FString AuthType;
	FParse::Value(FCommandLine::Get(), TEXT("AUTH_TYPE = "), AuthType);

	if (AuthType.IsEmpty() == false)
	{
		UE_LOG(LogTemp, Log, TEXT("Logging into EOS..."));

		if (Identity->AutoLogin(0) == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to login..."));

			Identity->ClearOnLoginCompleteDelegate_Handle(0, LoginDelegateHandle);
			LoginDelegateHandle.Reset();
		}
	}

	if (AuthType.IsEmpty() == true)
	{
		FOnlineAccountCredentials Credentials("developer", "127.0.0.1:8081", "lman888");

		UE_LOG(LogTemp, Log, TEXT("Logging into EOS..."));

		if (Identity->Login(0, Credentials) == false)
		{
			UE_LOG(LogTemp, Log, TEXT("Failed to login."));

			Identity->ClearOnLoginCompleteDelegate_Handle(0, LoginDelegateHandle);
			LoginDelegateHandle.Reset();
		}
	}
}

void ATDPlayerController::HandleLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserID, const FString& Error)
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	if (Subsystem == nullptr)
	{
		return;
	}

	IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
	if (Identity == nullptr)
	{
		return;
	}

	if (bWasSuccessful == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("EOS login failed."));
	}

	if (bWasSuccessful == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("EOS login completed."));
	}

	Identity->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, LoginDelegateHandle);
	LoginDelegateHandle.Reset();
}
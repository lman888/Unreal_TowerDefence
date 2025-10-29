// Fill out your copyright notice in the Description page of Project Settings.

#include "Instance/EOSGameInstance.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

UEOSGameInstance::UEOSGameInstance()
{

}

void UEOSGameInstance::Init()
{
	Super::Init();

	OnlineSubstem = IOnlineSubsystem::Get();
	if (OnlineSubstem == nullptr)
	{
		return;
	}
}

void UEOSGameInstance::CreateSession()
{
	if (OnlineSubstem == nullptr)
	{
		return;
	}

	IOnlineSessionPtr OnlineSessionPtr = OnlineSubstem->GetSessionInterface();
	if (OnlineSessionPtr == nullptr)
	{
		return;
	}

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.NumPublicConnections = 6;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bUsesPresence = true;

	OnlineSessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnCreateSessionComplete);
	OnlineSessionPtr->CreateSession(0, FName("Test Sessions"), SessionSettings);
}

void UEOSGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Success: [%d]"), bWasSuccessful);

	if (OnlineSubstem == nullptr)
	{
		return;
	}

	IOnlineSessionPtr OnlineSessionPtr = OnlineSubstem->GetSessionInterface();
	if (OnlineSessionPtr == nullptr)
	{
		return;
	}

	OnlineSessionPtr->ClearOnCreateSessionCompleteDelegates(this);
}
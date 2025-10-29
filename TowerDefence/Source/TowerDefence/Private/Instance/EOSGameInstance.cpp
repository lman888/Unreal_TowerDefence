// Fill out your copyright notice in the Description page of Project Settings.

#include "Instance/EOSGameInstance.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
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

	Login();
}

void UEOSGameInstance::Login()
{
	if (OnlineSubstem == nullptr)
	{
		return;
	}

	IOnlineIdentityPtr OnlineIdentity = OnlineSubstem->GetIdentityInterface();
	if (OnlineIdentity == nullptr)
	{
		return;	
	}

	FOnlineAccountCredentials AccountCredentials;
	AccountCredentials.Id = FString("127.0.0.1:8081");
	AccountCredentials.Token = FString("lman888");
	AccountCredentials.Type = FString("developer");
	OnlineIdentity->OnLoginCompleteDelegates->AddUObject(this, &UEOSGameInstance::OnLoginComplete);
	OnlineIdentity->Login(0, AccountCredentials);
}

void UEOSGameInstance::CreateSession()
{
	if (bIsLoggedIn == false)
	{
		return;
	}
	
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
	SessionSettings.bIsLANMatch = false;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bUsesPresence = true;

	OnlineSessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnCreateSessionComplete);
	OnlineSessionPtr->CreateSession(0, FName("Test Sessions"), SessionSettings);
}

void UEOSGameInstance::JoinSession(const FOnlineSessionSearchResult& SearchResult)
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

	OnlineSessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnJoinSessionCompleted);
	OnlineSessionPtr->JoinSession(0, FName("Test Sessions"), SearchResult);
}

void UEOSGameInstance::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
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

	FString ConnectString;
	if (OnlineSessionPtr->GetResolvedConnectString(SessionName, ConnectString))
	{
		UE_LOG(LogTemp, Log, TEXT("Joining Session at: [%s]"), *ConnectString);

		APlayerController* PlayerController = GetFirstLocalPlayerController();
		if (PlayerController == nullptr)
		{
			return;
		}

		PlayerController->ClientTravel(ConnectString, ETravelType::TRAVEL_Absolute);
	}

	OnlineSessionPtr->ClearOnJoinSessionCompleteDelegates(this);
}

void UEOSGameInstance::FindSession()
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
	
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = false;
	SessionSearch->MaxSearchResults = 10;
	SessionSearch->QuerySettings.Set(FName(TEXT("PRESENCESEARCH")), true, EOnlineComparisonOp::Equals);

	OnlineSessionPtr->OnFindSessionsCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnFindSessionCompleted);
	
	OnlineSessionPtr->FindSessions(0, SessionSearch.ToSharedRef());
}

void UEOSGameInstance::OnFindSessionCompleted(bool bWasSuccessful)
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

	UE_LOG(LogTemp, Warning, TEXT("Find Sessions Complete: Success=%d, Results=%d"), bWasSuccessful, SessionSearch->SearchResults.Num());

	if (bWasSuccessful == false)
	{
		return;
	}

	if (SessionSearch->SearchResults.Num() <= 0)
	{
		return;
	}

	JoinSession(SessionSearch->SearchResults[0]);

	OnlineSessionPtr->ClearOnFindSessionsCompleteDelegates(this);
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

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	World->ServerTravel("/Game/Levels/Map01?listen");
	
	OnlineSessionPtr->ClearOnCreateSessionCompleteDelegates(this);
}

void UEOSGameInstance::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserID, const FString& Error)
{
	UE_LOG(LogTemp, Warning, TEXT("Logged In Sucessful: [%d]"), bWasSuccessful);
	
	if (OnlineSubstem == nullptr)
	{
		return;
	}

	IOnlineIdentityPtr OnlineIdentity = OnlineSubstem->GetIdentityInterface();
	if (OnlineIdentity == nullptr)
	{
		return;	
	}

	bIsLoggedIn = bWasSuccessful;
	OnlineIdentity->ClearOnLoginCompleteDelegates(0, this);
}
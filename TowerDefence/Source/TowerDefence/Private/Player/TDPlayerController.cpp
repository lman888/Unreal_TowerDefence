// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TDPlayerController.h"

#include "OnlineSessionSettings.h"
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
	FParse::Value(FCommandLine::Get(), TEXT("AUTH_TYPE="), AuthType);

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
		FOnlineAccountCredentials Credentials("AccountPortal", "", "");

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
		FindSessions();
	}

	Identity->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, LoginDelegateHandle);
	LoginDelegateHandle.Reset();
}

void ATDPlayerController::FindSessions(FName SearchKey, FString SearchValue)
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	if (Subsystem == nullptr)
	{
		return;
	}
	
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
	if (Session == nullptr)
	{
		return;
	}
	
	TSharedRef<FOnlineSessionSearch> Search = MakeShared<FOnlineSessionSearch>();

	Search->QuerySettings.SearchParams.Empty();

	Search->QuerySettings.Set(SearchKey, SearchValue, EOnlineComparisonOp::Equals);
	FindSessionsDelegateHandle = Session->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ATDPlayerController::HandleFindSessionsCompleted, Search));

	UE_LOG(LogTemp, Log, TEXT("Finding session."));
 
	if (Session->FindSessions(0, Search) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Find session failed"));
	}
}

void ATDPlayerController::HandleFindSessionsCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search)
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	if (Subsystem == nullptr)
	{
		return;
	}
	
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
	if (Session == nullptr)
	{
		return;
	}

	if (bWasSuccessful == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Find Sessions failed."));
	}

	if (bWasSuccessful == true)
	{
		for (FOnlineSessionSearchResult SessionInSearchResult : Search->SearchResults)
		{
			if (Session->GetResolvedConnectString(SessionInSearchResult, NAME_GamePort, ConnectString))
			{
				SessionToJoin = &SessionInSearchResult; 
			}

			//Break after finding the first session for this example
			break;
		}
	}

	Session->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsDelegateHandle);
	FindSessionsDelegateHandle.Reset();
}

void ATDPlayerController::JoinSession()
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	if (Subsystem == nullptr)
	{
		return;
	}
	
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
	if (Session == nullptr)
	{
		return;
	}

	JoinSessionDelegateHandle = Session->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::HandleJoinSessionCompleted));

	UE_LOG(LogTemp, Log, TEXT("Joining session."));
	
	if (Session->JoinSession(0, "SessionName", *SessionToJoin) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Join session failed"));
	} 
}

void ATDPlayerController::HandleJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	if (Subsystem == nullptr)
	{
		return;
	}
	
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
	if (Session == nullptr)
	{
		return;
	}

	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Log, TEXT("Joined Session!"));

		if (GEngine)
		{
			//Change this to the IP of the server in future, for now its just the local host
			ConnectString = "127.0.0.1:8081";
			FURL DedicatedServerURL(nullptr, *ConnectString, TRAVEL_Absolute);
			FString DedicatedServerJoinError;

			EBrowseReturnVal::Type DedicatedServerJoinStatus = GEngine->Browse(GEngine->GetWorldContextFromWorldChecked(GetWorld()), DedicatedServerURL, DedicatedServerJoinError);

			if (DedicatedServerJoinStatus == EBrowseReturnVal::Failure)
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to browse for dedicated server. Error is: %s"), *DedicatedServerJoinError); 
			}
		}
	}

	Session->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionDelegateHandle);
	JoinSessionDelegateHandle.Reset();
}
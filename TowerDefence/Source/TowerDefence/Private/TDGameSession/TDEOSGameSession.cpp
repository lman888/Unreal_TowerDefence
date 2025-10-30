// Fill out your copyright notice in the Description page of Project Settings.


#include "TDGameSession/TDEOSGameSession.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemTypes.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/OnlineSessionInterface.h"


ATDEOSGameSession::ATDEOSGameSession()
{

}

void ATDEOSGameSession::BeginPlay()
{
	Super::BeginPlay();

	if (IsRunningDedicatedServer() && bSessionExists == false)
	{
		CreateSession("KeyName", "KeyValue");
	}
}

void ATDEOSGameSession::CreateSession(FName KeyName, FString KeyValue)
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

	CreateSessionDelegateHandle = Session->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ATDEOSGameSession::HandleCreateSessionCompleted));

	TSharedRef<FOnlineSessionSettings> SessionSettings = MakeShared<FOnlineSessionSettings>();
	SessionSettings->NumPublicConnections = MaxNumberOfPlayersInSession;
	SessionSettings->bShouldAdvertise = true;
	SessionSettings->bUsesPresence = false;
	SessionSettings->bAllowJoinViaPresence = false;
	SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;
	SessionSettings->bAllowInvites = false;
	SessionSettings->bAllowJoinInProgress = false;
	SessionSettings->bIsDedicated = true;
	SessionSettings->bUseLobbiesIfAvailable = false;
	SessionSettings->bUseLobbiesVoiceChatIfAvailable = false;
	SessionSettings->bUsesStats = true;

	//This custom attribute will be used in searches on Game Clients
	SessionSettings->Settings.Add(KeyName, FOnlineSessionSetting((KeyValue), EOnlineDataAdvertisementType::ViaOnlineService));

	UE_LOG(LogTemp, Log, TEXT("Creating Session..."));

	if (Session->CreateSession(0, SessionName, *SessionSettings) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Create Session!"));
		Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionDelegateHandle);
		CreateSessionDelegateHandle.Reset();
	}
}

void ATDEOSGameSession::HandleCreateSessionCompleted(FName EOSSessionName, bool bWasSuccessful)
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

	if (bWasSuccessful == true)
	{
		bSessionExists = true;
		UE_LOG(LogTemp, Log, TEXT("Session: [%s] Created."), *EOSSessionName.ToString());
	}

	if (bWasSuccessful == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Create Session."));
	}
	
	Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionDelegateHandle);
	CreateSessionDelegateHandle.Reset();
}

bool ATDEOSGameSession::ProcessAutoLogin()
{
	return Super::ProcessAutoLogin();
}

void ATDEOSGameSession::RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId,
	bool bWasFromInvite)
{
	Super::RegisterPlayer(NewPlayer, UniqueId, bWasFromInvite);

	if (IsRunningDedicatedServer() == false)
	{
		return;
	}

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

	RegisterPlayerDelegateHandle = Session->AddOnRegisterPlayersCompleteDelegate_Handle(FOnRegisterPlayersCompleteDelegate::CreateUObject(this, &ATDEOSGameSession::HandleRegisterPlayerCompleted));

	if (Session->RegisterPlayer(SessionName, *UniqueId, false) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Register Player!"));
		Session->ClearOnRegisterPlayersCompleteDelegate_Handle(RegisterPlayerDelegateHandle);
		RegisterPlayerDelegateHandle.Reset();
	}
}

void ATDEOSGameSession::HandleRegisterPlayerCompleted(FName EOSSessionName, const TArray<FUniqueNetIdRef>& PlayerIds,
	bool bWasSuccesful)
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

	if (bWasSuccesful == true)
	{
		UE_LOG(LogTemp, Log, TEXT("Player Registered in EOS Session"));
		NumberOfPlayersInSession++;

		if (NumberOfPlayersInSession == MaxNumberOfPlayersInSession)
		{
			StartSession();
		}
	}

	if (bWasSuccesful == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Register Player! (From Callback)"));
	}

	Session->ClearOnRegisterPlayersCompleteDelegate_Handle(RegisterPlayerDelegateHandle);
	RegisterPlayerDelegateHandle.Reset();
}

void ATDEOSGameSession::StartSession()
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

	StartSessionDelegateHandle = Session->AddOnStartSessionCompleteDelegate_Handle(FOnStartSessionCompleteDelegate::CreateUObject(this, &ATDEOSGameSession::HandleStartSessionCompleted));

	if (Session->StartSession(SessionName) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Start Session"));

		Session->ClearOnStartSessionCompleteDelegate_Handle(StartSessionDelegateHandle);
		StartSessionDelegateHandle.Reset();
	}
}

void ATDEOSGameSession::HandleStartSessionCompleted(FName EOSSessionName, bool bWasSuccessful)
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

	if (bWasSuccessful == true)
	{
		UE_LOG(LogTemp, Log, TEXT("Session Started!"));
	}

	if (bWasSuccessful == false)
	{
		UE_LOG(LogTemp, Log, TEXT("Failed to Start Session! (From Callback)"));
	}

	Session->ClearOnStartSessionCompleteDelegate_Handle(StartSessionDelegateHandle);
	StartSessionDelegateHandle.Reset();
}

void ATDEOSGameSession::UnregisterPlayer(const APlayerController* ExitingPlayer)
{
	Super::UnregisterPlayer(ExitingPlayer);

	if (IsRunningDedicatedServer() == false)
	{
		return;
	}

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

	IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
	if (Identity == nullptr)
	{
		return;
	}


	//If the player quits/leaves the game un-gracefully
	if (ExitingPlayer->PlayerState == true)
	{
		UnRegisterPlayerDelegateHandle = Session->AddOnUnregisterPlayersCompleteDelegate_Handle(FOnUnregisterPlayersCompleteDelegate::CreateUObject(this, &ATDEOSGameSession::HandleUnregisterPlayerCompleted));

		if (Session->UnregisterPlayer(SessionName, *ExitingPlayer->PlayerState->GetUniqueId()) == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to Unregister Player!"));

			Session->ClearOnUnregisterPlayersCompleteDelegate_Handle(UnRegisterPlayerDelegateHandle);
			UnRegisterPlayerDelegateHandle.Reset();
		}
	}

	if (ExitingPlayer->PlayerState == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Unregister Player!"));
		Session->ClearOnUnregisterPlayersCompleteDelegate_Handle(UnRegisterPlayerDelegateHandle);
		UnRegisterPlayerDelegateHandle.Reset();
	}
}

void ATDEOSGameSession::HandleUnregisterPlayerCompleted(FName EOSSessionName, const TArray<FUniqueNetIdRef>& PlayerIds,
	bool bWasSuccesful)
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

	if (bWasSuccesful == true)
	{
		UE_LOG(LogTemp, Log, TEXT("Player unregistered in EOS Session!"));
	}
	
	if (bWasSuccesful == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to unregister player! (From Callback)"));
	}
	Session->ClearOnUnregisterPlayersCompleteDelegate_Handle(UnRegisterPlayerDelegateHandle);
	UnRegisterPlayerDelegateHandle.Reset();
}

void ATDEOSGameSession::NotifyLogout(FName InSessionName, const FUniqueNetIdRepl& UniqueId)
{
	Super::NotifyLogout(InSessionName, UniqueId);

	if (IsRunningDedicatedServer() == false)
	{
		return;
	}

	NumberOfPlayersInSession--;
	if (NumberOfPlayersInSession == 0)
	{
		EndSession();
	}
}

void ATDEOSGameSession::EndSession()
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

	EndSessionDelegateHandle = Session->AddOnEndSessionCompleteDelegate_Handle(FOnEndSessionCompleteDelegate::CreateUObject(this, &ATDEOSGameSession::HandleEndSessionCompleted));

	if (Session->EndSession(SessionName) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to end session!"));
		Session->ClearOnEndSessionCompleteDelegate_Handle(StartSessionDelegateHandle);
		EndSessionDelegateHandle.Reset();
	}
}

void ATDEOSGameSession::HandleEndSessionCompleted(FName EOSSessionName, bool bWasSuccessful)
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
	
	if (bWasSuccessful == true)
	{
		UE_LOG(LogTemp, Log, TEXT("Session ended!"));
	}

	if (bWasSuccessful == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to end session! (From Callback)"));
	}
 
	Session->ClearOnEndSessionCompleteDelegate_Handle(EndSessionDelegateHandle);
	EndSessionDelegateHandle.Reset();
}

void ATDEOSGameSession::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	DestroySession();
}

void ATDEOSGameSession::DestroySession()
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

	DestroySessionDelegateHandle = Session->AddOnDestroySessionCompleteDelegate_Handle(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ATDEOSGameSession::HandleDestroySessionCompleted));

	if (Session->DestroySession(SessionName) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to destroy session."));
		Session->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionDelegateHandle);
		DestroySessionDelegateHandle.Reset();
	}
}

void ATDEOSGameSession::HandleDestroySessionCompleted(FName EOSSessionName, bool bWasSuccesful)
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
	
	if (bWasSuccesful == true)
	{
		bSessionExists = false;
		UE_LOG(LogTemp, Log, TEXT("Destroyed session succesfully.")); 
	}

	if (bWasSuccesful == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to destroy session.")); 
	}
 
	Session->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionDelegateHandle);
	DestroySessionDelegateHandle.Reset();
}
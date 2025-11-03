// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TDPlayerController.h"

#include "OnlineSessionSettings.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemTypes.h"
#include "Online/OnlineSessionNames.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"


ATDPlayerController::ATDPlayerController()
{
	bReplicates = true;
}

void ATDPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	//Login();
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

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	
	FUniqueNetIdPtr NetId = Identity->GetUniquePlayerId(LocalPlayer->GetUniqueID());
	if (NetId != nullptr && Identity->GetLoginStatus(*LocalPlayer->GetPreferredUniqueNetId()) == ELoginStatus::LoggedIn)
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

	TSharedPtr<FUserOnlineAccount> UserAccount = Identity->GetUserAccount(UserID);
	if (UserAccount == nullptr)
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
		UE_LOG(LogTemp, Warning, TEXT("EOS login failed."));
	}
	
	if (bWasSuccessful == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("EOS login completed: [%s]."), *UserAccount->GetDisplayName());
		
		FindSessions("KeyName", "TDGame");
	}

	Identity->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, LoginDelegateHandle);
	LoginDelegateHandle.Reset();
}

void ATDPlayerController::SetupNotifications()
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

	Session->AddOnSessionParticipantJoinedDelegate_Handle(FOnSessionParticipantJoinedDelegate::CreateUObject(this, &ThisClass::HandleParticipantJoined));
}

void ATDPlayerController::HandleParticipantJoined(FName EOSLobbyName, const FUniqueNetId& NetId)
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

	UE_LOG(LogTemp, Log, TEXT("Player Joined Lobby."));
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
#if P2PMODE

	Search->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);

#endif

	FindSessionsDelegateHandle = Session->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ATDPlayerController::HandleFindSessionsCompleted, Search));

#if P2PMODE
	UE_LOG(LogTemp, Log, TEXT("Finding lobby."));
#else 
	UE_LOG(LogTemp, Log, TEXT("Finding session."));
#endif


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
#if P2PMODE
		UE_LOG(LogTemp, Log, TEXT("Find lobby failed."));
#else 
		UE_LOG(LogTemp, Warning, TEXT("Find session failed."));
#endif 
	}

	if (bWasSuccessful == true)
	{

		if (Search->SearchResults.Num() == 0)
		{
#if P2PMODE
			GetWorld()->ServerTravel("/Game/Levels/Map01?listen", TRAVEL_Absolute);
			
			CreateLobby("KeyName", "TDGame");
#endif
			return;
		}
#if P2PMODE
		UE_LOG(LogTemp, Log, TEXT("Found lobby."));
#else 
		UE_LOG(LogTemp, Warning, TEXT("Found session."));
#endif 

		for (FOnlineSessionSearchResult SessionInSearchResult : Search->SearchResults)
		{
			if (Session->GetResolvedConnectString(SessionInSearchResult, NAME_GamePort, ConnectString))
			{
				SessionToJoin = MakeShared<FOnlineSessionSearchResult>(SessionInSearchResult);
			}

			//Break after finding the first session for this example
			break;
		}

		if (SessionToJoin->Session.OwningUserId)
		{
			JoinSession();
		}
	}

	Session->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsDelegateHandle);
	FindSessionsDelegateHandle.Reset();
}

void ATDPlayerController::CreateLobby(FName KeyName, FString KeyValue)
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

	CreateLobbyDelegateHandle = Session->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ATDPlayerController::HandleCreateLobbyCompleted));

	TSharedRef<FOnlineSessionSettings> SessionSettings = MakeShared<FOnlineSessionSettings>();
	SessionSettings->NumPublicConnections = 2;
	SessionSettings->bShouldAdvertise = true; //This creates a public match and will be searchable.
	SessionSettings->bUsesPresence = true;   //No presence on dedicated server. This requires a local user.
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;
	SessionSettings->bAllowInvites = false;    //Allow inviting players into session. This requires presence and a local user. 
	SessionSettings->bAllowJoinInProgress = false	; //Once the session is started, no one can join.
	SessionSettings->bIsDedicated = false; //Session created on dedicated server.
	SessionSettings->bUseLobbiesIfAvailable = true; //For P2P we will use a lobby instead of a session
	SessionSettings->bUseLobbiesVoiceChatIfAvailable = true; //We will also enable voice
	SessionSettings->bUsesStats = true; //Needed to keep track of player stats.
	SessionSettings->Settings.Add(KeyName, FOnlineSessionSetting((KeyValue), EOnlineDataAdvertisementType::ViaOnlineService));

	UE_LOG(LogTemp, Log, TEXT("Creating Lobby..."));

	if (Session->CreateSession(0, LobbyName, *SessionSettings) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create Lobby!"));
	}
}

void ATDPlayerController::HandleCreateLobbyCompleted(FName EOSLobbyName, bool bWasSuccessful)
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
		UE_LOG(LogTemp, Log, TEXT("Lobby: %s Created!"), *EOSLobbyName.ToString());
		
		Session->StartSession(EOSLobbyName);
		GetWorld()->ServerTravel("/Game/Levels/Map01?listen", TRAVEL_Absolute);

		SetupNotifications();
	}


	if (bWasSuccessful == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create lobby!"));
	}

	Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateLobbyDelegateHandle);
	CreateLobbyDelegateHandle.Reset();
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

#if P2PMODE
	UE_LOG(LogTemp, Log, TEXT("Joining Lobby."));
#else 
	UE_LOG(LogTemp, Log, TEXT("Joining session."));
#endif 

	if (SessionToJoin == nullptr)
	{
		return;
	}

	if (Session->JoinSession(0, LobbyName, *SessionToJoin) == false)
	{
#if P2PMODE
		UE_LOG(LogTemp, Log, TEXT("Join Lobby failed."));
#else 
		UE_LOG(LogTemp, Log, TEXT("Join session failed."));
#endif

		Session->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionDelegateHandle);
		JoinSessionDelegateHandle.Reset();
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

#if P2PMODE
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Log, TEXT("Joined lobby."));

		if (SessionToJoin == nullptr)
		{
			return;
		}
		
		FString HostIP = "127.0.0.1"; // or host’s LAN/public IP
		ConnectString = FString::Printf(TEXT("%s:%d"), *HostIP, 7777);

		FSessionSettings SessionSettings = SessionToJoin->Session.SessionSettings.Settings;
		// Log basic session info
		UE_LOG(LogTemp, Log, TEXT("Session Owning User ID: %s"), *SessionToJoin->Session.OwningUserId->ToString());
		UE_LOG(LogTemp, Log, TEXT("Session Owning User Name: %s"), *SessionToJoin->Session.OwningUserName);
		
		if (Session->GetResolvedConnectString(*SessionToJoin, NAME_GamePort, ConnectString))
		{
			UE_LOG(LogTemp, Log, TEXT("Travel URL: %s"), *ConnectString);
			Session->JoinSession(0, LobbyName, *SessionToJoin);
			ClientTravel(ConnectString, TRAVEL_Absolute);
		}
		
		SetupNotifications(); // Setup our listeners for lobby event notifications

	}
#else
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
#endif

	Session->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionDelegateHandle);
	JoinSessionDelegateHandle.Reset();
}
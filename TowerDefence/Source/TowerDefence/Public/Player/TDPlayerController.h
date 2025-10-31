// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "TDPlayerController.generated.h"

class FOnlineSessionSearch;
class FOnlineSessionSearchResult;

/**
 *
 */
UCLASS()
class TOWERDEFENCE_API ATDPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATDPlayerController();

protected:
	virtual void BeginPlay();

	void Login();

	void HandleLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserID, const FString& Error);

	void CreateLobby(FName KeyName = "KeyName", FString KeyValue = "KeyValue");

	void HandleCreateLobbyCompleted(FName EOSLobbyName, bool bWasSuccessful);

	void SetupNotifications();

	void HandleParticipantJoined(FName EOSLobbyName, const FUniqueNetId& NetId);

	void FindSessions(FName SearchKey = "KeyName", FString SearchValue = "KeyValue");

	void HandleFindSessionsCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search);

	void JoinSession();

	void HandleJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	FDelegateHandle FindSessionsDelegateHandle;

	FString ConnectString;

	FName LobbyName = "TD Game";

	FOnlineSessionSearchResult* SessionToJoin;
	FDelegateHandle JoinSessionDelegateHandle;
	FDelegateHandle LoginDelegateHandle;
	FDelegateHandle CreateLobbyDelegateHandle;
};
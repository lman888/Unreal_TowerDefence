// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "TDEOSGameSession.generated.h"

/**
 *
 */
UCLASS()
class TOWERDEFENCE_API ATDEOSGameSession : public AGameSession
{
	GENERATED_BODY()

public:
	ATDEOSGameSession();

	virtual void BeginPlay() override;

	void CreateSession(FName KeyName = "KeyName", FString KeyValue = "KayValue");

	void HandleCreateSessionCompleted(FName EOSSessionName, bool bWasSuccessful);

	virtual void RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId, bool bWasFromInvite) override;

	void HandleRegisterPlayerCompleted(FName EOSSessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful);

	void StartSession();

	void HandleStartSessionCompleted(FName EOSSessionName, bool bWasSuccessful);

	virtual void UnregisterPlayer(const APlayerController* ExitingPlayer) override;

	void HandleUnregisterPlayerCompleted(FName EOSSessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful);

	virtual void NotifyLogout(FName InSessionName, const FUniqueNetIdRepl& UniqueId) override;

	void EndSession();

	void HandleEndSessionCompleted(FName EOSSessionName, bool bWasSuccessful);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void DestroySession();

	void HandleDestroySessionCompleted(FName EOSSessionName, bool bWasSuccesful);
	
protected:
	FDelegateHandle CreateSessionDelegateHandle;
	FDelegateHandle RegisterPlayerDelegateHandle;
	FDelegateHandle StartSessionDelegateHandle;
	FDelegateHandle UnRegisterPlayerDelegateHandle;
	FDelegateHandle EndSessionDelegateHandle;
	FDelegateHandle DestroySessionDelegateHandle;

	FName SessionName = "TDGame";

	const FString Map = TEXT("/Game/Levels/Map01"); 

	int32 MaxNumberOfPlayersInSession = 6;
	
	int NumberOfPlayersInSession = 0;

	bool bSessionExists = false;
};

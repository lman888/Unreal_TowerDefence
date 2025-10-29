// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "EOSGameInstance.generated.h"

class IOnlineSubsystem;

/**
 *
 */
UCLASS()
class TOWERDEFENCE_API UEOSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UEOSGameInstance();

	virtual void Init() override;

	void Login();

	UFUNCTION(BlueprintCallable)
	void CreateSession();

	void JoinSession(const FOnlineSessionSearchResult& SearchResult);

	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION(BlueprintCallable)
	void FindSession();

	void OnFindSessionCompleted(bool bWasSuccessful);
	
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserID, const FString& Error);

protected:
	IOnlineSubsystem* OnlineSubstem;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	
	bool bIsLoggedIn = false;
};

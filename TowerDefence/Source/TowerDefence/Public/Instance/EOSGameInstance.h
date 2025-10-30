// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "GameFramework/GameSession.h"
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

protected:
	IOnlineSubsystem* OnlineSubsystem;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	bool bIsLoggedIn = false;
	bool bSessionExists = false;

	FDelegateHandle CreateSessionDelegateHandle;
};

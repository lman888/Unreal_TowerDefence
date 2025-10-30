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

	void CreateSession(FName KeyName = "KeyName", FString KeyValue = "KayValue");

	void HandleCreateSessionCompleted(FName EOSSessionName, bool bWasSuccessful);

protected:
	FDelegateHandle CreateSessionDelegateHandle;
};

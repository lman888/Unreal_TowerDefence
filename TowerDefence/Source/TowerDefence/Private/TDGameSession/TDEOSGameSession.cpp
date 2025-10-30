// Fill out your copyright notice in the Description page of Project Settings.


#include "TDGameSession/TDEOSGameSession.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemTypes.h"
#include "Interfaces/OnlineSessionInterface.h"


ATDEOSGameSession::ATDEOSGameSession()
{

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


}

void ATDEOSGameSession::HandleCreateSessionCompleted(FName EOSSessionName, bool bWasSuccessful)
{
}

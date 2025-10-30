// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/TDGameMode.h"
#include "TDGameSession/TDEOSGameSession.h"

ATDGameMode::ATDGameMode()
{
	GameSessionClass = ATDEOSGameSession::StaticClass();
}
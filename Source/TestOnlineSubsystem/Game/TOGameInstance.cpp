// Fill out your copyright notice in the Description page of Project Settings.


#include "TOGameInstance.h"

#include "OnlineKeyValuePair.h"
#include "OnlineSessionSettings.h"

UTOGameInstance::UTOGameInstance()
{
}

void UTOGameInstance::Init()
{
	UWorld* World = GetWorld();

	if (World)
	{
		if (World->GetNetMode() == NM_DedicatedServer)
		{
			UE_LOG(LogTemp, Error, TEXT("Init - NM_DedicatedServer"));

			GetOnlineSubsystem();
			CreateGameSession();
		}
	}
}

void UTOGameInstance::GetOnlineSubsystem()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

	if (OnlineSubsystem)
	{
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Subsystem Name : % s"), *OnlineSubsystem->GetSubsystemName().ToString()));
		}
	}
}

void UTOGameInstance::CreateGameSession()
{
	if (!OnlineSessionInterface.IsValid()) {
		return;
	}

	auto ExistingSession = OnlineSessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr) {
		OnlineSessionInterface->DestroySession(NAME_GameSession);
	}

	OnlineSessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &ThisClass::OnCreateSessionComplete);

	TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
	SessionSettings->bIsLANMatch = true;
	SessionSettings->NumPublicConnections = 4;
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->bShouldAdvertise = true;
	SessionSettings->bUsesPresence = true;
	
	SessionSettings->bIsDedicated = true;
	
	// 개별 값 추가
	SessionSettings->Set(FName(TEXT("SessionName")), FString(TEXT("DedicatedServer Session 1")), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings->Set(FName(TEXT("MatchType")),FString(TEXT("Deathmatch")), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
	OnlineSessionInterface->CreateSession(0, FName(TEXT("DedicatedServer Session 1")), *SessionSettings);
}

void UTOGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Error, TEXT("Complete Create Seesion : % s"), *SessionName.ToString());
}

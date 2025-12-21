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
	SessionSettings->bUsesPresence = false;           // 필수 수정
	SessionSettings->bAllowJoinViaPresence = false;   // Presence 비활성화
	SessionSettings->bShouldAdvertise = true;         // 서버 목록 노출
	SessionSettings->bIsDedicated = true;             // 유지
	SessionSettings->bIsLANMatch = false;             // 인터넷 매치

	
	// 개별 값 추가
	SessionSettings->Set(FName(TEXT("SessionName")), FString(TEXT("DedicatedServer Session")), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings->Set(FName(TEXT("MatchType")),FString(TEXT("Deathmatch")), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings->Set(FName(TEXT("SessionStart")), false, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
	OnlineSessionInterface->CreateSession(0, FName(TEXT("DedicatedServer Session")), *SessionSettings);
}

void UTOGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Error, TEXT("Complete Create Seesion : % s"), *SessionName.ToString());

	FNamedOnlineSession* Session = OnlineSessionInterface->GetNamedSession(SessionName);
	if (Session)
	{

	}
}

void UTOGameInstance::ManageSession(bool bIsSessionStarted)
{
	FNamedOnlineSession* Session = OnlineSessionInterface->GetNamedSession(FName(TEXT("DedicatedServer Session")));
	UE_LOG(LogTemp, Error, TEXT("%s Session is closing..."), *Session->SessionName.ToString());

	if (Session)
	{
		FOnlineSessionSettings& Settings = Session->SessionSettings;
		
		
		Settings.Set(FName(TEXT("SessionStart")), bIsSessionStarted, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		OnlineSessionInterface->UpdateSession(Session->SessionName, Settings);
	}
}

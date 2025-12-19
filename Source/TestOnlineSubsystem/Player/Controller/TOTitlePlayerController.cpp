// Fill out your copyright notice in the Description page of Project Settings.


#include "TOTitlePlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Online/OnlineSessionNames.h" // FName

#include "UI/TitleHUD.h"

ATOTitlePlayerController::ATOTitlePlayerController()
	: CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ATOTitlePlayerController::OnCreateSessionComplete))
	, FindSessionCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionComplete))
	, JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplate))
{
}

void ATOTitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}

	ENetMode NetMode = GetWorld()->GetNetMode();
	if (NetMode == NM_ListenServer)
	{
		UE_LOG(LogTemp, Warning, TEXT("ATOTitlePlayerController- NM_ListenServer"));
	}
	else if (NetMode == NM_DedicatedServer)
	{
		UE_LOG(LogTemp, Warning, TEXT("ATOTitlePlayerController- NM_DedicatedServer"));
	}
	else if (NetMode == NM_Client)
	{
		UE_LOG(LogTemp, Warning, TEXT("ATOTitlePlayerController- NM_Client"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ATOTitlePlayerController- else: stand alone"));
	}

	if (IsValid(WBP_Title_class) == true)
	{
		WBP_Title_Instance = CreateWidget<UTitleHUD>(this, WBP_Title_class);
		if (IsValid(WBP_Title_Instance) == true)
		{
			WBP_Title_Instance->AddToViewport();

			FInputModeUIOnly Mode;
			SetInputMode(Mode);

			bShowMouseCursor = true;
		}
	}

	GetOnlineSubsystem();
}

void ATOTitlePlayerController::GetOnlineSubsystem()
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

void ATOTitlePlayerController::CreateGameSession()
{
	if (!OnlineSessionInterface.IsValid()) {
		return;
	}

	auto ExistingSession = OnlineSessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr) {
		OnlineSessionInterface->DestroySession(NAME_GameSession);
	}

	OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
	SessionSettings->bIsLANMatch = true;
	SessionSettings->NumPublicConnections = 4;
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->bShouldAdvertise = true;
	SessionSettings->bUsesPresence = true;

	// 개별 값 추가
	SessionSettings->Set(FName(TEXT("SessionName")), FString(TEXT("ListenServer Session")), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings->Set(FName(TEXT("MatchType")), FString(TEXT("Deathmatch")), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), FName(TEXT("ListenServer Session")), *SessionSettings);
}

void ATOTitlePlayerController::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful) {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Blue,
				FString::Printf(TEXT("Created session: %s"), *SessionName.ToString())
			);
		}

		UWorld* World = GetWorld();
		if (World) 
		{
			World->ServerTravel(FString("/Game/TestOnlineSubsystem/Level/ThirdPersonMap?listen"));
		}
			
	}
}

void ATOTitlePlayerController::ReadyToFindSession()
{
	if (!OnlineSessionInterface.IsValid())
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("Game Session Interface is invailed")));

		return;
	}

	OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteDelegate);

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->bIsLanQuery = false;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);


	UE_LOG(LogTemp, Error, TEXT("Find Sessions..."));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("Find Sessions...")));
	}

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
}

void ATOTitlePlayerController::OnFindSessionComplete(bool bWasSuccessful)
{
	if (!OnlineSessionInterface.IsValid() || !bWasSuccessful)
	{
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("Found Session Count : %d"), SessionSearch->SearchResults.Num());
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("Found Session Count : %d"), SessionSearch->SearchResults.Num()));
	}

	for (auto Result : SessionSearch->SearchResults)
	{
		FString Id = Result.GetSessionIdStr();
		FString User = Result.Session.OwningUserName;
		
		FString SessionName;
		Result.Session.SessionSettings.Get(FName("SessionName"), SessionName);
		FString MatchType;
		Result.Session.SessionSettings.Get(FName("MatchType"), MatchType);

		bool bSessionStart;
		Result.Session.SessionSettings.Get(FName("SessionStart"), bSessionStart);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("Session: %s(%s)- %s"), *SessionName, *Id, (bSessionStart ? TEXT("Started") : TEXT("Watting"))));
		}

		//if (MatchType == FString("FreeForAll"))
		//if(SessionName == FString(TEXT("DedicatedServer Session 2")))
		if(true && bSessionStart == false)
		{
			OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

			const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
			if (IsValid(LocalPlayer))
			{
				OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), FName(*SessionName), Result);
				return;
			}
		}
	}
}

void ATOTitlePlayerController::OnJoinSessionComplate(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!OnlineSessionInterface.IsValid())
	{
		return;
	}

	FString Address;
	if (OnlineSessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("Joined Session Name : %s(%s)"),*SessionName.ToString(),  *Address));
		}

		APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
		if (PlayerController)
		{
			PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
		}
	}
}



void ATOTitlePlayerController::JoinLocalGame()
{ 
	APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
	if (PlayerController)
	{
		PlayerController->ClientTravel("127.0.0.1:7777", ETravelType::TRAVEL_Absolute);
	}
}

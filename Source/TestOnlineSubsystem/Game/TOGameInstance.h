// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"


#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

#include "TOGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TESTONLINESUBSYSTEM_API UTOGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UTOGameInstance();

public:
	virtual void Init() override;

protected:
	void GetOnlineSubsystem();
	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> OnlineSessionInterface;

# pragma region CreateSession
public:
	UFUNCTION(BlueprintCallable)
	void CreateGameSession();
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);


private:
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
#pragma endregion

# pragma region Open/Cloase Session

public:
	void ManageSession(bool bIsSessionStarted);

# pragma endregion
	
};

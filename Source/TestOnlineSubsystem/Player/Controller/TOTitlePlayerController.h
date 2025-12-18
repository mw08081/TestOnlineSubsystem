// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"


#include "TOTitlePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TESTONLINESUBSYSTEM_API ATOTitlePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ATOTitlePlayerController();

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UTitleHUD> WBP_Title_class;

	UPROPERTY()
	TObjectPtr<class UTitleHUD> WBP_Title_Instance;

protected:
	void GetOnlineSubsystem();

	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> OnlineSessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

# pragma region CreateSession
public:
	UFUNCTION(BlueprintCallable)
	void CreateGameSession();

private:
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
#pragma endregion

# pragma region FindSession
public:
	UFUNCTION(BlueprintCallable)
	void ReadyToFindSession();

private:
	FOnFindSessionsCompleteDelegate FindSessionCompleteDelegate;
	void OnFindSessionComplete(bool bWasSuccessful);

#pragma endregion

# pragma region JoinSession

	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	void OnJoinSessionComplate(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

#pragma endregion

public:
	UFUNCTION(BlueprintCallable)
	void JoinLocalGame ();
};

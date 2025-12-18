// Fill out your copyright notice in the Description page of Project Settings.


#include "TOGameModeBase.h"


void ATOGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GetOnlineSubsystem();
}

void ATOGameModeBase::GetOnlineSubsystem()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

	if (OnlineSubsystem)
	{
		SessionInterface = OnlineSubsystem->GetSessionInterface();

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Purple, FString::Printf(TEXT("Subsystem Name : % s"), *OnlineSubsystem->GetSubsystemName().ToString()));
	}
}
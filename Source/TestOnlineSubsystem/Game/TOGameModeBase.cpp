// Fill out your copyright notice in the Description page of Project Settings.


#include "TOGameModeBase.h"

#include "Player/Controller/TOTitlePlayerController.h"
#include "Game/TOGameInstance.h"

void ATOGameModeBase::BeginPlay()
{
	Super::BeginPlay();

}

void ATOGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ATOTitlePlayerController* NewPlayerController = Cast<ATOTitlePlayerController>(NewPlayer);
	if (NewPlayerController != nullptr)
	{
		AlivePlayerControllers.Add(NewPlayerController);

		if (AlivePlayerControllers.Num() >= 2) {
			UE_LOG(LogTemp, Error, TEXT("Player is full. This Session will be closed..."));

			UTOGameInstance* GI = GetWorld()->GetGameInstance<UTOGameInstance>();
			if (GI)
			{
				GI->ManageSession(true);
			}
		}
	}
}


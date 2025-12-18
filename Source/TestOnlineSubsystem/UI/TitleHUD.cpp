// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleHUD.h"

#include "Components/Button.h"

#include "Player/Controller/TOTitlePlayerController.h"

void UTitleHUD::NativeConstruct()
{
	Button_CreateListenServer.Get()->OnClicked.AddDynamic(this, &UTitleHUD::OnCreateListenServerClicked);
	Button_SearchSession.Get()->OnClicked.AddDynamic(this, &UTitleHUD::OnSearchSessionClicked);
}

void UTitleHUD::OnCreateListenServerClicked()
{
	ATOTitlePlayerController* OwningPlayerController = GetOwningPlayer<ATOTitlePlayerController>();
	if (IsValid(OwningPlayerController))
	{
		OwningPlayerController->CreateGameSession();
	}
}

void UTitleHUD::OnSearchSessionClicked()
{
	ATOTitlePlayerController* OwningPlayerController = GetOwningPlayer<ATOTitlePlayerController>();
	if (IsValid(OwningPlayerController))
	{
		OwningPlayerController->ReadyToFindSession();
	}
}
 
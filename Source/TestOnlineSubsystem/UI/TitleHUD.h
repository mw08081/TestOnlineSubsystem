// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleHUD.generated.h"


class UButton;
/**
 * 
 */
UCLASS()
class TESTONLINESUBSYSTEM_API UTitleHUD : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCreateListenServerClicked();

	UFUNCTION()
	void OnSearchSessionClicked();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButton> Button_CreateListenServer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButton> Button_SearchSession;
};

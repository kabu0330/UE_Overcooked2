// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPlayerController.h"

#include "Global/OC2GameInstance.h"
#include "Global/Manager/LobbyManager.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI Mode;
	SetInputMode(Mode);

	SetShowMouseCursor(true);
}

void ALobbyPlayerController::Client_SetUserIndex_Implementation(int InUserIndex)
{
	UOC2GameInstance* GameInstance = Cast<UOC2GameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance)
	{
		GameInstance->SetUserIndex(InUserIndex);
	}
}

// ALobbyPlayerController.cpp
void ALobbyPlayerController::Client_SetChefHeadName_Implementation(const FString& NewChefHeadName)
{
	UOC2GameInstance* GameInstance = Cast<UOC2GameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance)
	{
		GameInstance->SetChefHeadName(NewChefHeadName);
	}
}
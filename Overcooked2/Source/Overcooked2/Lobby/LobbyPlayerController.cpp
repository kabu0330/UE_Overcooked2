// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeUIOnly Mode;
	SetInputMode(Mode);

	SetShowMouseCursor(true);
}

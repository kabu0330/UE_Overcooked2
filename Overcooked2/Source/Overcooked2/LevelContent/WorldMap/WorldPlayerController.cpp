// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelContent/WorldMap/WorldPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

void AWorldPlayerController::AddInputMappingContext(UInputMappingContext* _Context)
{
	if (GetLocalPlayer() == nullptr)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	InputSystem->ClearAllMappings();
	InputSystem->AddMappingContext(_Context, 0);
}
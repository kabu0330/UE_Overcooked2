// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OC2PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void AOC2PlayerController::BeginPlay()
{
	UEnhancedInputLocalPlayerSubsystem* InputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(InputLocalPlayerSubsystem != nullptr)
	{
		InputLocalPlayerSubsystem->AddMappingContext(MappingContext, 0);
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

}

void AOC2PlayerController::SetupInputComponent()
{
}

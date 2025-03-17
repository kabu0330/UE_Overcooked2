// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/TableTest/TTController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


//void ATTController::SetupInputComponentEvent()
//{
//
//}

void ATTController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* InputSystem = 
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	InputSystem->ClearAllMappings();

	InputSystem->AddMappingContext(MappingContext, 0);
}

//void ATTController::AddMappingContext(UInputMappingContext* MappingContext)
//{
//
//}

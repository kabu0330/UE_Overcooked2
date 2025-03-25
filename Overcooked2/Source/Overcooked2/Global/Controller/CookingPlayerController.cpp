// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/Controller/CookingPlayerController.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


ACookingPlayerController::ACookingPlayerController()
{
}

ACookingPlayerController::~ACookingPlayerController()
{
}

void ACookingPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly Mode;
	SetInputMode(Mode);

	UEnhancedInputLocalPlayerSubsystem* InputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (InputLocalPlayerSubsystem != nullptr)
	{
		InputLocalPlayerSubsystem->AddMappingContext(MappingContext, 0);
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
}

void ACookingPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACookingPlayerController::SetupInputComponent()
{
}

void ACookingPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);


}

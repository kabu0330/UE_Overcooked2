// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/DevLevel/GlobalDevPlayerController.h"

void AGlobalDevPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly Mode;
	SetInputMode(Mode);
}

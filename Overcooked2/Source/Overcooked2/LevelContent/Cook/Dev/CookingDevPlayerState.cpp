// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Dev/CookingDevPlayerState.h"
#include <LevelContent/Cook/Dev/SpawnManagerComponent.h>
#include <Net/UnrealNetwork.h>
#include <LevelContent/Cook/Dev/CookingDevGameMode.h>

ACookingDevPlayerState::ACookingDevPlayerState()
{
	bReplicates = true;
	SpawnManagerComponent = CreateDefaultSubobject<USpawnManagerComponent>(TEXT("SpawnManagerComponent"));
}

void ACookingDevPlayerState::SetIngredientType(EIngredientType Type)
{
	IngredientType = Type;
	RequestSpawnIngredient();
}

void ACookingDevPlayerState::RequestSpawnIngredient_Implementation()
{
	ACookingDevGameMode* GameMode = Cast<ACookingDevGameMode>(GetWorld()->GetAuthGameMode());
	if (nullptr != GameMode)
	{
		GameMode->SpawnIngredient(IngredientType);
	}
}

void ACookingDevPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void ACookingDevPlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACookingDevPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACookingDevPlayerState, IngredientType);
}

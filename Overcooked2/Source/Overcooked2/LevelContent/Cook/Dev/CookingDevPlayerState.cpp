// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Dev/CookingDevPlayerState.h"
#include <LevelContent/Cook/Dev/SpawnManagerComponent.h>
#include <Net/UnrealNetwork.h>
#include <LevelContent/Cook/Dev/CookingDevGameMode.h>
#include <LevelContent/Cook/Ingredient.h>

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

void ACookingDevPlayerState::ChangeState_Implementation(EIngredientState State)
{
	ACookingDevGameMode* GameMode = Cast<ACookingDevGameMode>(GetWorld()->GetAuthGameMode());
	if (nullptr != GameMode)
	{
		GameMode->ChangeState(State);
	}
}

void ACookingDevPlayerState::ChangeStateLogic(EIngredientState State)
{
	if (true == CookingActor.IsEmpty())
	{
		return;
	}

	IngredientState = State;
	for (int i = 0; i < CookingActor.Num(); i++)
	{
		AIngredient* Ingredient = Cast<AIngredient>(CookingActor[i]);
		if (IngredientState == Ingredient->GetCurIngredientState())
		{
			return;
		}
		else
		{
			Ingredient->ChangeState(IngredientState);
		}
	}
}

void ACookingDevPlayerState::AddPlayerState_Implementation()
{
	ACookingDevGameMode* GameMode = Cast<ACookingDevGameMode>(GetWorld()->GetAuthGameMode());
	if (nullptr != GameMode)
	{
		GameMode->AddPlayerState(this);
	}
}

void ACookingDevPlayerState::BeginPlay()
{
	Super::BeginPlay();

	AddPlayerState();
}

void ACookingDevPlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACookingDevPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACookingDevPlayerState, IngredientType);
	DOREPLIFETIME(ACookingDevPlayerState, CookingActor);
	DOREPLIFETIME(ACookingDevPlayerState, IngredientState);
}

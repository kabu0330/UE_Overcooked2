// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Dev/SpawnManagerComponent.h"
#include <Net/UnrealNetwork.h>
#include <LevelContent/Cook/Dev/CookingDevGameMode.h>

// Sets default values for this component's properties
USpawnManagerComponent::USpawnManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}

void USpawnManagerComponent::SetIngredientType(EIngredientType Type)
{
	IngredientType = Type;
	RequestSpawnIngredient(IngredientType);
}

void USpawnManagerComponent::RequestSpawnIngredient_Implementation(EIngredientType Type)
{
	ACookingDevGameMode* GameMode = Cast<ACookingDevGameMode>(GetWorld()->GetAuthGameMode());
	if (nullptr != GameMode)
	{
		IngredientType = Type;
		GameMode->SpawnIngredient(IngredientType);
	}
}


// Called when the game starts
void USpawnManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USpawnManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USpawnManagerComponent, IngredientType);
}


// Called every frame
void USpawnManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


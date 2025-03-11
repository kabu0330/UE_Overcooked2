// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/Component/SpawnManageComponent.h"
#include "Overcooked2.h"

#include "LevelContent/Cook/Ingredient.h"
#include "LevelContent/Cook/Plate.h"

USpawnManageComponent::USpawnManageComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

AIngredient* USpawnManageComponent::SpawnIngredientActor(EIngredientType IngredientType, TSubclassOf<AIngredient> IngredientToSpawn)
{
	FTransform IngredientTransform;
	AIngredient* IngredientActor = GetWorld()->SpawnActorDeferred<AIngredient>(IngredientToSpawn, IngredientTransform);

	if (nullptr == IngredientActor)
	{
		UE_LOG(OVERCOOKED_LOG, Error, TEXT("Ingredient actor spawn failed"));

		return nullptr;
	}

	//IngredientActor->SetType(IngredientType);

	IngredientActor->FinishSpawning(IngredientTransform);

	return IngredientActor;
}

APlate* USpawnManageComponent::SpawnPlateActor(TSubclassOf<APlate> PlateTopSpawn)
{
	FTransform PlateTransform;
	APlate* PlateActor = GetWorld()->SpawnActorDeferred<APlate>(PlateTopSpawn, PlateTransform);

	if (nullptr == PlateActor)
	{
		UE_LOG(OVERCOOKED_LOG, Error, TEXT("Plate actor spawn failed"));

		return nullptr;
	}

	PlateActor->FinishSpawning(PlateTransform);

	return PlateActor;
}


void USpawnManageComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void USpawnManageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
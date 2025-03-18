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

AIngredient* USpawnManageComponent::SpawnIngredientActor(EIngredientType IngredientType)
{
	FTransform IngredientTransform;
	AIngredient* IngredientActor = GetWorld()->SpawnActorDeferred<AIngredient>(AIngredient::StaticClass(), IngredientTransform);
	
	if (nullptr == IngredientActor)
	{
		UE_LOG(OVERCOOKED_LOG, Error, TEXT("Ingredient actor spawn failed"));

		return nullptr;
	}

	IngredientActor->SetType(IngredientType);

	FVector IngredientLocation = FVector(0.0f, 0.0f, 100.0f);
	IngredientTransform.SetLocation(IngredientLocation);

	IngredientActor->FinishSpawning(IngredientTransform);

	return IngredientActor;
}


APlate* USpawnManageComponent::SpawnPlateActor(TSubclassOf<APlate> PlateTopSpawn, EPlateState PlateState)
{
	FTransform PlateTransform;
	APlate* PlateActor = GetWorld()->SpawnActorDeferred<APlate>(PlateTopSpawn, PlateTransform);

	if (nullptr == PlateActor)
	{
		UE_LOG(OVERCOOKED_LOG, Error, TEXT("Plate actor spawn failed"));

		return nullptr;
	}

	PlateActor->SetPlateState(PlateState);

	PlateActor->FinishSpawning(PlateTransform);

	return PlateActor;
}


void USpawnManageComponent::BeginPlay()
{
	Super::BeginPlay();

	//FString IngredientPath = TEXT("/Game/Blueprints/LevelContent/Cooking/BP_Ingredient.BP_Ingredient_C");
	//FString PlatePath = TEXT("/Game/Blueprints/LevelContent/Cooking/BP_Plate.BP_Plate_C");

	//IngredientToSpawn = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *IngredientPath));
	//PlateToSpawn = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *PlatePath));

	//if (IngredientToSpawn)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("IngredientToSpawn successfully loaded: %s"), *IngredientToSpawn->GetName());
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Failed to load IngredientToSpawn!"));
	//}

	//if (PlateToSpawn)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("PlateToSpawn successfully loaded: %s"), *PlateToSpawn->GetName());
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Failed to load PlateToSpawn!"));
	//}

}

void USpawnManageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
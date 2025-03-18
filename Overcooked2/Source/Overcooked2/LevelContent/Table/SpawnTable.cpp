// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/SpawnTable.h"
#include <Global/Data/IngredientDataTable.h>
#include "Kismet/GameplayStatics.h"
#include "Global/GameMode/OC2GameMode.h"
#include <Character/OC2Character.h>

ASpawnTable::ASpawnTable()
{
}

void ASpawnTable::BeginPlay()
{
	Super::BeginPlay();

}

void ASpawnTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawnTable::SetIngredient(EIngredientType IngredientTypeSetting)
{
	IngredientType = IngredientTypeSetting;
}



ACooking* ASpawnTable::Interact(AActor* ChefActor)
{
	AOC2Character* Chef = Cast<AOC2Character>(ChefActor);
	ACooking* TempCooking = nullptr;
	
	if (false == bIsOccupied) // 테이블이 비어있다.
	{
		RequestSpawn();
		return SpawnedIngredient;
	}
	else
	{
		//TempCooking = SPlacedItem;
		TempCooking = CookingPtr;
		//PlacedItem이 외부 Cooking과 상호작용 하는 경우
		// 1. 조합 가능한 경우
		// 2. 빈접시와 요리가 있는 접시의 경우
	}

	return TempCooking;
}

void ASpawnTable::RequestSpawn_Implementation()
{
	auto GM = Cast<AOC2GameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		SpawnedIngredient = GM->SpawnIngredientActor(IngredientType);
	}
}

void ASpawnTable::PlaceItem(ACooking* Item)
{
	CookingPtr = Item;
	
	FVector OnTheTable = GetActorLocation() + FVector{ (0.0f, 0.0f, 100.0f) };
	CookingPtr->SetActorLocation(OnTheTable);
}

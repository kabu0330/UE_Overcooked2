// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/PrepTable.h"
#include <Character/OC2Character.h>

APrepTable::APrepTable()
{

}

void APrepTable::BeginPlay()
{
	Super::BeginPlay();
}

void APrepTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ACooking* APrepTable::Interact(AActor* ChefActor)
{
	AOC2Character* Chef = Cast<AOC2Character>(ChefActor);

	if (false == bIsOccupied) // 테이블이 비어있다.
	{
		PPlacedItem = nullptr;
	}
	return PPlacedItem;
}

void APrepTable::PlaceItem(ACooking* Item)
{
	PPlacedItem = Item;
	FVector OnTheTable = GetActorLocation() + FVector{ (0.0f, 0.0f, 100.0f) };

	PPlacedItem->SetActorLocation(OnTheTable);
}

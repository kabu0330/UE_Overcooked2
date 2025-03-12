// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/BurnerTable.h"

ABurnerTable::ABurnerTable()
{

}

void ABurnerTable::BeginPlay()
{
	Super::BeginPlay();
}

void ABurnerTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ACooking* ABurnerTable::Interact(AActor* ChefActor)
{
	return BPlacedItem;
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/ChoppingTable.h"

AChoppingTable::AChoppingTable()
{

}

void AChoppingTable::BeginPlay()
{
	Super::BeginPlay();
}

void AChoppingTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ACooking* AChoppingTable::Interact(AActor* ChefActor)
{
	return CPlacedItem;
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/NonTable/ServingTable.h"
#include "LevelContent/Cook/Plate.h"

AServingTable::AServingTable()
{

}

void AServingTable::BeginPlay()
{
	Super::BeginPlay();
}

void AServingTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AServingTable::PlaceItem(ACooking* ReceivedCooking)
{
	if (ECookingType::ECT_PLATE == ReceivedCooking->GetCookingType())
	{
		CookingPtr = ReceivedCooking;
	}
}

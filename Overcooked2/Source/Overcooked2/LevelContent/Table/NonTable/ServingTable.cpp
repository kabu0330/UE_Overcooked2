// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/NonTable/ServingTable.h"
#include "LevelContent/Cook/Plate.h"

#include "Global/OC2Global.h"

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
	if (nullptr == ReceivedCooking)
	{
		return;
	}

	if (ECookingType::ECT_PLATE == ReceivedCooking->GetCookingType())
	{
	}
	else
	{
		return;
	}

	Super::PlaceItem(ReceivedCooking);

	CookingPtr->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	UOC2Global::SubmitPlate(GetWorld(), ReceivedCooking);

	CookingPtr = nullptr;
}

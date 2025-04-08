// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/NonTable/PlateSpawner.h"
#include "LevelContent/Cook/Plate.h"

APlateSpawner::APlateSpawner()
{

}

void APlateSpawner::BeginPlay()
{
	Super::BeginPlay();
}

void APlateSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlateSpawner::SetPlate(class APlate* Plate)
{
	if (nullptr != CookingPtr)
	{
		PlateMap.Add(PlateNum, CookingPtr);
		for (int i = 0; i < PlateNum; i++)
		{
			PlateMap[i]->AddActorLocalOffset(FVector::UnitZ() * 30.0f);
		}
		CookingPtr = nullptr;
		PlateNum++;
	}

	CookingPtr = Cast<APlate>(Plate);
	CookingPtr->SetCookingTable_Implementation(this);

	//Cooking을 Attach 시킬것
	CookingPtr->AttachToComponent(ComponentForCooking, FAttachmentTransformRules::KeepRelativeTransform);
	CookingPtr->SetActorLocation(ComponentForCooking->GetComponentLocation());
}

ACooking* APlateSpawner::Interact(AActor* ChefActor)
{
	if (nullptr == CookingPtr && PlateNum > 0)
	{
		CookingPtr = PlateMap[PlateNum - 1];
		PlateMap.Remove(PlateNum - 1);
		PlateNum--;
	}
	ACooking* CookingReturn = CookingPtr;
	CookingPtr = nullptr;
	return CookingReturn;
}

void APlateSpawner::PlaceItem(ACooking* ReceivedCooking)
{
	ACooking* TempCooking = ReceivedCooking;
}
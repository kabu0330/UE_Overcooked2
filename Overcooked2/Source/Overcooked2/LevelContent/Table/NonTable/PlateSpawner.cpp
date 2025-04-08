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
	CookingPtr = Cast<APlate>(Plate);
	CookingPtr->SetCookingTable_Implementation(this);

	PlateNum++;
	PlateMap.Add(PlateNum, CookingPtr);

	PlateMap[PlateNum]->AttachToComponent(ComponentForCooking, FAttachmentTransformRules::KeepRelativeTransform);
	PlateMap[PlateNum]->SetActorLocation(ComponentForCooking->GetComponentLocation());
	PlateMap[PlateNum]->AddActorLocalOffset(FVector::UnitZ() * 30.0f * (PlateNum -1));

	CookingPtr = nullptr;
}

ACooking* APlateSpawner::Interact(AActor* ChefActor)
{
	if (PlateNum > 0)
	{
		CookingPtr = PlateMap[PlateNum];
		PlateMap.Remove(PlateNum);
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
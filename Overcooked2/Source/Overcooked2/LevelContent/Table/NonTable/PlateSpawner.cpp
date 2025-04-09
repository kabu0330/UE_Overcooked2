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

	if (nullptr == Plate)
	{
		return;
	}

	if (nullptr == OwnedPlate) // 
	{
		OwnedPlate = Plate;
		OwnedPlate->AttachToComponent(ComponentForCooking, FAttachmentTransformRules::KeepRelativeTransform);
		OwnedPlate->SetActorLocation(ComponentForCooking->GetComponentLocation());
		OwnedPlate->AddActorLocalOffset(FVector(0, 0, 0));
	}
	else
	{
		OwnedPlate->StackPlate(Plate);
		if (1 == OwnedPlate->GetAnotherPlatesRef().Num())
		{
			OwnedPlate->AddActorLocalOffset(FVector(0, 0, 10));
		}
		if (3 == OwnedPlate->GetAnotherPlatesRef().Num())
		{
			OwnedPlate->AddActorLocalOffset(FVector(0, 0, 50));
		}
	}
	//PlateNum++;
	//PlateMap.Add(PlateNum, CookingPtr);
	//PlateMap[PlateNum]->SetActorLocation(ComponentForCooking->GetComponentLocation());
	//PlateMap[PlateNum]->AddActorLocalOffset(FVector::UnitZ() * 30.0f * (PlateNum -1));

	CookingPtr = nullptr;
}

ACooking* APlateSpawner::Interact(AActor* ChefActor)
{
	if (nullptr != OwnedPlate) // 내가 접시를 스폰한게 있으면
	{
		ACooking* NewCooking = Cast<ACooking>(OwnedPlate);
		if (nullptr != NewCooking)
		{
			// 접시를 캐릭터에게 넘겨주고 나는 가지고 있는 접시가 없다.
			OwnedPlate = nullptr; 
			return NewCooking;
		}
	}

	// 내가 가지고 있는 접시가 없다면 널 반환
	return nullptr;

	//if (PlateNum > 0)
	//{
	//	CookingPtr = PlateMap[PlateNum];
	//	PlateMap.Remove(PlateNum);
	//	PlateNum--;
	//}
	//ACooking* CookingReturn = CookingPtr;
	//CookingPtr = nullptr;
	//return CookingReturn;
}

void APlateSpawner::PlaceItem(ACooking* ReceivedCooking)
{
	ACooking* TempCooking = ReceivedCooking;
}
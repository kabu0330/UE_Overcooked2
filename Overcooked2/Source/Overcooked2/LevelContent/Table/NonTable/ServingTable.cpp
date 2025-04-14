// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/NonTable/ServingTable.h"
#include "LevelContent/Cook/Plate.h"
#include "LevelContent/Table/NonTable/PlateSpawner.h"

#include "Global/OC2Global.h"
#include "Global/GameMode/OC2GameMode.h"

#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"

AServingTable::AServingTable()
{

}

void AServingTable::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ACookingTable> It(GetWorld()); It; ++It)
	{
		ACookingTable* PrepTableActor = *It;
		if (PrepTableActor->Tags.Contains("PlateSpawner"))
		{
			PlateSpawner = Cast<APlateSpawner>(PrepTableActor);
		}
	}
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

	Server_SpawnPlateInPlateSpawner();

	CookingPtr = nullptr;
}

void AServingTable::Server_SpawnPlateInPlateSpawner_Implementation()
{
	auto GameMode = Cast<AOC2GameMode>(GetWorld()->GetAuthGameMode());
	APlate* Plate = nullptr;
	if (GameMode)
	{
		Plate = GameMode->SpawnPlateActor(EPlateState::DIRTY);
	}
	//Plate->AttachToChef(PlateSpawner);
	//PlateSpawner->PlaceItem(Plate);
	//PlateSpawner->SetPlate(Plate);
	Plate->Multicast_SubmitPlate();
	//PlateSpawner->AddPlate(int Num1);
}
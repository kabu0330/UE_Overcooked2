// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/PrepTable.h"
#include <Character/OC2Character.h>
#include <Global/GameMode/OC2GameMode.h>

APrepTable::APrepTable()
{

}

void APrepTable::BeginPlay()
{
	Super::BeginPlay();

	if (bSpawnWhenGameStarted == true && HasAuthority())
	{
		RequestSpawnPlate();
	}
}

void APrepTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ACooking* APrepTable::Interact(AActor* ChefActor)
{
	AOC2Character* Chef = Cast<AOC2Character>(ChefActor);
	ACooking* TempCooking = nullptr;

	if (CookingPtr == nullptr && false == Chef->IsHolding()) // 테이블이 비어있다.
	{
		return nullptr;
	}
	else
	{
		TempCooking = CookingPtr;
		CookingPtr = nullptr;

		return TempCooking;
	}
}

void APrepTable::RequestSpawnPlate_Implementation()
{
	auto GameMode = Cast<AOC2GameMode>(GetWorld()->GetAuthGameMode());
	APlate* Plate = nullptr;
	if (GameMode)
	{
		Plate = GameMode->SpawnPlateActor(EPlateState::EMPTY);
	}
	Plate->AttachToChef(this);
	Plate->SetActorLocation(ComponentForCooking->GetComponentLocation());
	CookingPtr = Plate;
}



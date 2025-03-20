// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/BurnerTable.h"
#include <Character/OC2Character.h>
#include <LevelContent/Cook/Pot.h>
#include <Global/OC2Enum.h>

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
	AOC2Character* Chef = Cast<AOC2Character>(ChefActor);
	ACooking* TempCooking = nullptr;

	if (CookingPtr != nullptr && false == Chef->IsHolding()) // À½½Ä ÀÖÀ½, ¼ÎÇÁ ºó ¼Õ
	{
		TempCooking = CookingPtr;
		CookingPtr = nullptr;
		return TempCooking;
	}
	else
	{
		return CookingPtr;
	}
}

//void ABurnerTable::PlaceItem(ACooking* Item)
//{
//	ACooking* TempCooking = Item;
//	ACookingTable::PlaceItem(Item);
//	CookingPtr = TempCooking;
//
//	if (true == CookingPtr->IsCookingType(ECookingType::ECT_POT))
//	{
//		/*FVector OnTheTable = GetActorLocation() + FVector{ (0.0f, 0.0f, 100.0f) };
//		TempCooking->SetActorLocation(OnTheTable);*/
//		/*CookingPtr = TempCooking;
//		ACookingTable::PlaceItem(Item);*/
//
//		APot* TempPot = Cast<APot>(CookingPtr);
//		//TempPot->SetOnTable(true);
//	}
//}

//void ABurnerTable::BoilThePot(AActor* ChefActor)
//{
//	if (nullptr != ChefActor)
//	{
//
//	}
//}
// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/CookingTable.h"
#include "Global/GameMode/OC2GameMode.h"
//#include "Global/Component/TimeEventComponent.h"

// Sets default values
ACookingTable::ACookingTable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	ComponentForCooking = CreateDefaultSubobject<USceneComponent>("Attachment");
	ComponentForCooking->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACookingTable::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void ACookingTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACookingTable::PlaceItem(ACooking* ReceivedCooking)
{
	CookingPtr = ReceivedCooking;
	CookingPtr->SetCookingTable_Implementation(this);

	//Cooking을 Attach 시킬것
	CookingPtr->AttachToComponent(ComponentForCooking, FAttachmentTransformRules::KeepRelativeTransform);
	CookingPtr->SetActorLocation(ComponentForCooking->GetComponentLocation());

	bIsOccupied = true;
}


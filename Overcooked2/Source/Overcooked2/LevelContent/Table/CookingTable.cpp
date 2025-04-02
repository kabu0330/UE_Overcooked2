// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/CookingTable.h"
#include "Global/GameMode/OC2GameMode.h"
#include <Global/Data/IngredientDataTable.h>
#include <Global/OC2GameInstance.h>
#include "Kismet/GameplayStatics.h"
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
	CookingPtr->AttachToComponent(ComponentForCooking, FAttachmentTransformRules::KeepRelativeTransform);

	//AIngredient* TempIngredient = Cast<AIngredient>(CookingPtr);
	//if (nullptr != TempIngredient)
	//{
	//	SetIngredientOffset(TempIngredient);
	//	CookingPtr->GetStaticMeshComponent()->SetRelativeRotation(IngreRotation);
	//	CookingPtr->GetStaticMeshComponent()->SetRelativeLocation(IngreLocation);
	//}

	CookingPtr->SetActorLocation(ComponentForCooking->GetComponentLocation());
	bIsOccupied = true;
}

void ACookingTable::SetIngredientOffset(AIngredient* Ingredient)
{
	EIngredientType Type = Ingredient->GetIngredientType();
	UOC2GameInstance* GameInst = Cast<UOC2GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	FName Name = GameInst->GetIngredientDataTableRowName(Type);

	const FIngredientDataRow* IngredientDataTable = &GameInst->GetIngredientDataRow(Name);
	IngreRotation = IngredientDataTable->Rotation;
	IngreLocation = IngredientDataTable->Location;
}

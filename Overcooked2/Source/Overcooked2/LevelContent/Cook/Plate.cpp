// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Plate.h"
#include <LevelContent/Cook/Ingredient.h>

// Sets default values
APlate::APlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CookingType = ECookingType::ECT_PLATE;

	IngredientMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IngredientMesh"));
	PlateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlateMesh"));
	RootComponent = PlateMesh;

	FVector Offset = GetActorLocation() + FVector(0.0f, 0.0f, 30.0f);
	IngredientMesh->AddRelativeLocation(Offset);

}

// Called when the game starts or when spawned
void APlate::BeginPlay()
{
	Super::BeginPlay();
	LoadDataTable();
}

// Called every frame
void APlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlate::LoadDataTable()
{
	CookingDataTable;
	IngredientDataTable;
}

bool APlate::IsDirtyPlate()
{
	return PlateState == EPlateState::DIRTY;
}

void APlate::WashPlate()
{
	if (true == IsDirtyPlate())
	{
		PlateState = EPlateState::EMPTY;
	}
}

bool APlate::Add(AIngredient* Ingredient)
{
	if (EIngredientState::EIS_NONE == Ingredient->GetCurIngredientState())
	{
		return false;
	}
	if (nullptr == PlateMesh)
	{
		return false;
	}

	Ingredient->GetStaticMeshComponent()->SetStaticMesh(nullptr);
	Ingredient->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	Ingredient->SetActorLocation(FVector(0.0f, 0.0f, 0.0f));

	Ingredients.Add(Ingredient);
	//IngredientMesh->SetStaticMesh();

	CookCheck();
	return true;
}

void APlate::CookCheck()
{

}
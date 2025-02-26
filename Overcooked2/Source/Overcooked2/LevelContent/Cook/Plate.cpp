// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Plate.h"
#include <LevelContent/Cook/Ingredient.h>

// Sets default values
APlate::APlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APlate::CleanPlate()
{
	{
		if (nullptr != IngredientMesh)
		{
			return;
		}
	}
}

bool APlate::Add(AIngredient* Ingredient)
{
	if (Ingredient->GetCurIngredientState() == EIngredientState::EIS_NONE)
	{
		return false;
	}


	Ingredient->GetStaticMeshComponent()->SetStaticMesh(nullptr);
	Ingredient->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	Ingredient->SetActorLocation(FVector(0.0f, 0.0f, 0.0f));

	Ingredients.Add(Ingredient);

	CookCheck();
	return true;
}

void APlate::CookCheck()
{
}

// Called when the game starts or when spawned
void APlate::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Dev/CookingDevGameMode.h"
#include <LevelContent/Cook/Ingredient.h>
#include <LevelContent/Cook/Dev/CookingDevHUD.h>
#include <LevelContent/Cook/Dev/CookingDevUserWidget.h>
#include <Global/OC2Global.h>
#include <LevelContent/Cook/Plate.h>


ACookingDevGameMode::ACookingDevGameMode()
{
	CookingObjectManager = CreateDefaultSubobject<UCookingObjectManager>(TEXT("CookingObjectManager"));
}

void ACookingDevGameMode::ChangeState(EIngredientState State)
{
	TArray<AIngredient*>& Ingredients = CookingObjectManager->GetIngredients();

	if (true == Ingredients.IsEmpty())
	{
		return;
	}

	EIngredientState IngredientState = State;
	for (int i = 0; i < Ingredients.Num(); i++)
	{
		AIngredient* Ingredient = Cast<AIngredient>(Ingredients[i]);
		if (IngredientState == Ingredient->GetCurIngredientState())
		{
			continue;
		}
		else
		{
			Ingredient->ChangeState(IngredientState);
		}
	}
}

void ACookingDevGameMode::PlaceOnthePlate()
{
	TArray<AIngredient*>& Ingredients = CookingObjectManager->GetIngredients();

	if (true == Ingredients.IsEmpty())
	{
		return;
	}
	AIngredient* TargetIngredient = Ingredients[0];

	if (false == TargetIngredient->IsCooked())
	{
		return;
	}
	TArray<APlate*>& Plates = CookingObjectManager->GetPlates();
	if (true ==  Plates.IsEmpty())
	{
		return;
	}
	Plates[0]->Add(TargetIngredient);
	if (true == Plates[0]->IsCombinationSuccessful())
	{
		// 재료 추가 성공 시
		Ingredients.RemoveAt(0);
		return;
	}

	// 재료 추가 실패 시
	int a = 0;
}

void ACookingDevGameMode::PlaceOnThePot()
{
	TArray<AIngredient*>& Ingredients = CookingObjectManager->GetIngredients();

	if (true == Ingredients.IsEmpty())
	{
		return;
	}
	AIngredient* TargetIngredient = Ingredients[0];

	if (nullptr != Pot)
	{
		if (EIngredientType::EIT_RICE == TargetIngredient->GetIngredientType() && EIngredientState::EIS_BOILABLE == TargetIngredient->GetCurIngredientState())
		{
			Ingredients.RemoveAt(0);
		}

		Pot->Add(TargetIngredient);
	}

}

void ACookingDevGameMode::Wash()
{
	TArray<APlate*>& Plates = CookingObjectManager->GetPlates();
	if (true == Plates.IsEmpty())
	{
		return;
	}
	Plates[0]->WashPlate();
}

void ACookingDevGameMode::StackPlate()
{
	TArray<APlate*>& Plates = CookingObjectManager->GetPlates();
	if (true == Plates.IsEmpty()) 
	{
		return;
	}
	if (nullptr == StackBasePlate)
	{
		StackBasePlate = Plates[0];
		StackBasePlate->SetActorLocation(PlateInitPos + FVector(-100, 0, 0));
		Plates.RemoveAt(0);
		return;
	}

	StackBasePlate->StackPlate(Plates[0]);
	Plates.RemoveAt(0);

}

void ACookingDevGameMode::CleanPlate()
{
	TArray<APlate*>& Plates = CookingObjectManager->GetPlates();
	if (true == Plates.IsEmpty())
	{
		return;
	}
	Plates[0]->CleanPlate();
}

void ACookingDevGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (nullptr != SubclassPot)
	{
		Pot = GetWorld()->SpawnActor<APot>(SubclassPot);
		Pot->SetActorLocation(FVector(-400, 0, 10));
	}
	
}

void ACookingDevGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

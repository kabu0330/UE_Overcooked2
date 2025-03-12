// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Dev/CookingDevGameMode.h"
#include <LevelContent/Cook/Ingredient.h>
#include <LevelContent/Cook/Dev/CookingDevHUD.h>
#include <LevelContent/Cook/Dev/CookingDevUserWidget.h>
#include <Global/OC2Global.h>
#include <LevelContent/Cook/Plate.h>

ACookingDevGameMode::ACookingDevGameMode()
{
}

void ACookingDevGameMode::SpawnIngredient(EIngredientType Type)
{
	// 2. 서버에서 스폰한다.
	//UWorld* World = GetWorld();
	FTransform Trans;
	AIngredient* Ingredient = GetWorld()->SpawnActorDeferred<AIngredient>(AIngredient::StaticClass(), Trans);
	
	// 3. 월드에 편입하여 BeginPlay 호출 전에 기본 설정을 해준다.
	Ingredient->SetType(Type);

	FVector Location = FVector(0.0f, 0.0f, 100.0f);
	Trans.SetLocation(Location);

	Ingredient->FinishSpawning(Trans); // 4. BeginPlay 호출

	for (int i = 0; i < PlayerState.Num(); i++)
	{
		PlayerState[i]->AddCookingActor(Ingredient);
	}

}

void ACookingDevGameMode::SpawnPlate()
{
	APlate* Plate = GetWorld()->SpawnActor<APlate>(APlate::StaticClass());

}

void ACookingDevGameMode::ChangeState(EIngredientState State)
{
	for (int i = 0; i < PlayerState.Num(); i++)
	{
		PlayerState[i]->ChangeStateLogic(State);
	}
}

void ACookingDevGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ACookingDevGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

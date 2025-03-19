// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/TableTest/TableTestCharacter.h"
#include <LevelContent/Cook/Ingredient.h>
#include <LevelContent/Cook/Plate.h>
#include <LevelContent/Table/CookingTable.h>

// Sets default values
ATableTestCharacter::ATableTestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATableTestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATableTestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATableTestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATableTestCharacter::TestInteract(AIngredient* Ingredient)
{
	TestTable->Interact(this);
}

void ATableTestCharacter::TestPlaceItem(AIngredient* Ingredient)
{
	TestTable->PlaceItem(TestCooking);
}

//void ATableTestCharacter::MakeIngredient(EIngredientType IngredientTypeSetting)
//{
//	IngredientType = IngredientTypeSetting;
//}
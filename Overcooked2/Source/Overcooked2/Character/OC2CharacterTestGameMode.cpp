// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OC2CharacterTestGameMode.h"
#include "Character/TestItemManager.h"
#include "Character/Oc2CharacterTestTable.h"
#include "LevelContent/Cook/Ingredient.h"
#include "Net/UnrealNetwork.h"

AOC2CharacterTestGameMode::AOC2CharacterTestGameMode()
    :Super()
{
    ItemManger = CreateDefaultSubobject<UTestItemManager>(TEXT("ItemManager"));
}

AIngredient* AOC2CharacterTestGameMode::SpawnIngredient(EIngredientType Type)
{
    return ItemManger->CreateIngredient(EIngredientType::EIT_FISH);
}



void AOC2CharacterTestGameMode::BeginPlay()
{
    Super::BeginPlay();

    AActor* NewDropItem = ItemManger->CreateIngredient(EIngredientType::EIT_FISH);

    if (nullptr != NewDropItem)
    {
        NewDropItem->SetActorLocation(FVector(100, 100, 10.0));
    }
}






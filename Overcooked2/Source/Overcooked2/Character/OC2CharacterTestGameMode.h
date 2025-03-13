// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LevelContent/Cook/Ingredient.h"
#include "OC2CharacterTestGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API AOC2CharacterTestGameMode : public AGameMode
{
	GENERATED_BODY()
public:
    AOC2CharacterTestGameMode();

    AIngredient* SpawnIngredient(EIngredientType Type);


protected:
    void BeginPlay() override;

    

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UTestItemManager* ItemManger = nullptr;
};

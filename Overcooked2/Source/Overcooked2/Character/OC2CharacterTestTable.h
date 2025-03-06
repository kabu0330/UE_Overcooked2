// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelContent/Table/CookingTable.h"
#include "Character/OC2CharacterTestObject.h"
#include "OC2CharacterTestTable.generated.h"

UCLASS()
class OVERCOOKED2_API AOC2CharacterTestTable : public ACookingTable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOC2CharacterTestTable();

	virtual ACooking* Interact(AActor* ChefActor) override;
	AOC2CharacterTestObject* SpawnIngredient(AActor* ChefActor);
	virtual void PlaceItem(ACooking* Cook);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	ACooking* CurCook = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserComponents")
	USceneComponent* MySceneComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	EIngredientType IngredientType;
};

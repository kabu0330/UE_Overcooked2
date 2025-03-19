// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TableTestCharacter.generated.h"
//#include "Global/OC2Enum.h"

UCLASS()
class OVERCOOKED2_API ATableTestCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATableTestCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class AIngredient* TestIngredient = nullptr;

	class ACooking* TestCooking = nullptr;

	class ACookingTable* TestTable = nullptr;

	//EIngredientType IngredientType = EIngredientType::EIT_NONE;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void TestInteract(class AIngredient* Ingredient);

	UFUNCTION(BlueprintCallable)
	void TestPlaceItem(class AIngredient* Ingredient);

	/*UFUNCTION(BlueprintCallable)
	void MakeIngredient(EIngredientType IngredientTypeSetting);*/

	
};

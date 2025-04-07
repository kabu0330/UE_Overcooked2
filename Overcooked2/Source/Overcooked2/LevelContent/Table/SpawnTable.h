// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelContent/Table/CookingTable.h"
#include <Global/OC2Enum.h>
#include <Global/Component/TimeEventComponent.h>
#include "SpawnTable.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ASpawnTable : public ACookingTable
{
	GENERATED_BODY()

public:
	ASpawnTable();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetIngredient(EIngredientType IngredientTypeSetting);

	UFUNCTION(BlueprintCallable)
	virtual ACooking* Interact(AActor* ChefActor) override;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void RequestSpawn();

	/*UFUNCTION(BlueprintCallable)
	virtual void PlaceItem(ACooking* Item) override;*/
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AnimMontage = nullptr;*/

protected:

private:

	EIngredientType IngredientType = EIngredientType::EIT_NONE;

	AIngredient* SpawnedIngredient = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bCanAnimation = false;

	bool bIsAnimation = false;

	UTimeEventComponent* TimeEventComponent = nullptr;
};

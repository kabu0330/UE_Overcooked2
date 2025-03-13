// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include <Global/OC2Enum.h>
#include <LevelContent/Cook/Cooking.h>
#include <LevelContent/Cook/Plate.h>
#include "CookingDevPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ACookingDevPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ACookingDevPlayerState();

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void SetIngredientType(EIngredientType Type);
	void SetIngredientType_Implementation(EIngredientType Type);

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void RequestSpawnIngredient();
	void RequestSpawnIngredient_Implementation();

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void SpawnPlate();
	void SpawnPlate_Implementation();

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void ChangeState(EIngredientState State);
	void ChangeState_Implementation(EIngredientState State);

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void PlaceOnthePlate();
	void PlaceOnthePlate_Implementation();

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void Reset();
	void Reset_Implementation();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	EIngredientType IngredientType = EIngredientType::EIT_NONE;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	EIngredientState IngredientState = EIngredientState::EIS_NONE;


};

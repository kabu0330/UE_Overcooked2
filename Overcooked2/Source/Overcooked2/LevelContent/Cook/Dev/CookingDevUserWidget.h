// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <LevelContent/Cook/Cooking.h>
#include <Global/OC2Enum.h>
#include "CookingDevUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UCookingDevUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Network
	UFUNCTION(BlueprintCallable)
	void ServerSpawnIngredient(EIngredientType Type);

	UFUNCTION(BlueprintCallable)
	void SpawnPlate(bool bIsClean);

	UFUNCTION(BlueprintCallable)
	void ChangeState(EIngredientState State);

	// Plate에서 Add 함수 호출하여 Ingredient 추가
	UFUNCTION(BlueprintCallable)
	void PlaceOnthePlate();

	UFUNCTION(BlueprintCallable)
	void PlaceOnthePot();

	UFUNCTION(BlueprintCallable)
	void Wash();


	UFUNCTION(BlueprintCallable)
	void StackPlate();


	UFUNCTION(BlueprintCallable)
	void CleanPlate();


	UFUNCTION(BlueprintCallable)
	void Reset();


	UFUNCTION(BlueprintCallable)
	void StartServer();

	UFUNCTION(BlueprintCallable)
	void Connect();

protected:
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network", meta = (AllowPrivateAccess = "true"))
	FString IP = TEXT("127.0.0.1");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network", meta = (AllowPrivateAccess = "true"))
	FString Port = TEXT("30000");
};

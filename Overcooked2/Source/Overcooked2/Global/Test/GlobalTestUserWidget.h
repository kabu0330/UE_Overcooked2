// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GlobalTestUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UGlobalTestUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Global|Test")
	void CreateIngredientActor();

	UFUNCTION(BlueprintCallable, Category = "Global|Test")
	void CreatePlateActor();
};

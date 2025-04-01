// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IngredientIconWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UIngredientIconWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void SetTexture(class UTexture2D* NewTexture);

protected:
    UPROPERTY(meta = (BindWidget))
    class UImage* IconImage = nullptr;
	
};

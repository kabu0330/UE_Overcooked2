// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlateIconWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UPlateIconWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    void Init();
    
    UFUNCTION(BlueprintCallable)
    void SetIngredientTextures(const TArray<UTexture2D*>& InTextures);

    UFUNCTION(BlueprintCallable)
    void Reset();

protected:
    UPROPERTY(meta = (BindWidget))
    class UImage* ImageSlot1;

    UPROPERTY(meta = (BindWidget))
    class UImage* ImageSlot2;

    UPROPERTY(meta = (BindWidget))
    class UImage* ImageSlot3;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "CookStatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UCookStatusWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void SetStatusTexture(class UTexture2D* NewTexture);

protected:
    UPROPERTY(meta = (BindWidget))
    class UImage* StatusImage = nullptr;
};

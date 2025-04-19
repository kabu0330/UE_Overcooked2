// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CookingHUD.generated.h"

class UCookingWidget;

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ACookingHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	void InitCookWidget();

public:
	UPROPERTY(EditAnywhere, Category = "UI|Cooking")
	UCookingWidget* CookWidget = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "UI|Cooking")
	TSubclassOf<UCookingWidget> CookWidgetObject;
};

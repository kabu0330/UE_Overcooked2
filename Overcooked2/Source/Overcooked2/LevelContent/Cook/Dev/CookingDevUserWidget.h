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

	UFUNCTION(BlueprintCallable)
	void SetTargetActor(ACooking* Actor)
	{
		if (nullptr != CookingActor)
		{
			CookingActor = nullptr;
		}
		CookingActor = Actor;
	}

	UFUNCTION(BlueprintCallable)
	void ChangeState(EIngredientState State);

protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	ACooking* CookingActor = nullptr;
};

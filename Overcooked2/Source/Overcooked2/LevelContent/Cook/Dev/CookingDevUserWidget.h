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
	void AddTargetActor(ACooking* Actor)
	{
		if (false == CookingActor.IsEmpty())
		{
			ACooking* NewActor = CookingActor.Last();
			FVector Offset = FVector(0.0f, 50.0f, 0.0f);
			FVector AdjustLocation = Offset + NewActor->GetActorLocation();
			Actor->SetActorLocation(AdjustLocation);
		}
	
		CookingActor.Add(Actor);
	}

	UFUNCTION(BlueprintCallable)
	TArray<ACooking*>& GetTargetActor()
	{
		return CookingActor;
	}

	UFUNCTION(BlueprintCallable)
	ACooking* GetTargetActorIndex(int Index)
	{
		return CookingActor[Index];
	}

	UFUNCTION(BlueprintCallable)
	void Reset()
	{
		CookingActor.Empty();
	}

	UFUNCTION(BlueprintCallable)
	void ChangeState(EIngredientState State);

protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	TArray<ACooking*> CookingActor;
};

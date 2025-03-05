// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelContent/Table/CookingTable.h"
#include "GarbageCan.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API AGarbageCan : public ACookingTable
{
	GENERATED_BODY()

public:
	AGarbageCan();

	UFUNCTION(BlueprintCallable)
	virtual ACooking* Interact(AActor* ChefActor) override 
	{
		return nullptr; 
	};

	UFUNCTION(BlueprintCallable)
	virtual void PlaceItem(ACooking* Item) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeSetting", meta = (AllowPrivateAccess = "true"))
	float Timer = 0.0f;
	
};

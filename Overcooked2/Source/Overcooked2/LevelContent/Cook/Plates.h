// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelContent/Cook/Cooking.h"
#include <LevelContent/Cook/Plate.h>
#include "Plates.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API APlates : public ACooking
{
	GENERATED_BODY()

public:
	APlates();

	UFUNCTION(NetMulticast, Reliable)
	void Add(APlate* Plate);
	void Add_Implementation(APlate* Plate);

	bool IsCombinationSuccessful() const
	{
		return bIsCombinationSuccessful;
	}

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	void SetRelativePosition();

private:
	UPROPERTY(Replicated)
	TArray<APlate*> Plates;

	UPROPERTY(Replicated)
	bool bIsCombinationSuccessful = false;
	
};

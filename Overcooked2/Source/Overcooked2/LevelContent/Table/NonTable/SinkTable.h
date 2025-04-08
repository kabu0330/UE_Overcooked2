// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelContent/Table/CookingTable.h"
#include "SinkTable.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ASinkTable : public ACookingTable
{
	GENERATED_BODY()

public:
	ASinkTable();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual ACooking* Interact(AActor* ChefActor) override;

	virtual void PlaceItem(ACooking* ReceivedCooking) override;

	void DoTheDishes(class AOC2Character* ChefActor);

	void WashingIsDone();
	
	void CheckChefIsWashing();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking")
	USceneComponent* ComponentForProgressBar = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CleanPlate")
	USceneComponent* CleanPlateComponent = nullptr;

	class AOC2Character* ChefPtr = nullptr;

protected:

	float Timer = 0.0f;
	bool bTimerActivated = false;
	bool bWashingDone = false;

	float Ratio = 0.0f;

	UPROPERTY()
	class UWidgetComponent* ProgressBarComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Table", meta = (AllowprivateAccess = "true"))
	TSubclassOf<UUserWidget> TSubClassWidget = nullptr;

	class UGaugeTextureWidget* WidgetPtr = nullptr;

};

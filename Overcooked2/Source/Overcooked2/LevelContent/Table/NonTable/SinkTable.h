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

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual ACooking* Interact(AActor* ChefActor) override;

	virtual void PlaceItem(ACooking* ReceivedCooking) override;

	void DoTheDishes(class AOC2Character* ChefActor);

	void WashingIsDone();
	
	void CheckChefIsWashing();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void HideProgressBar(bool Value);
	void HideProgressBar_Implementation(bool Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking")
	USceneComponent* ComponentForProgressBar = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CleanPlate")
	USceneComponent* CleanPlateComponent = nullptr;

	UPROPERTY(Replicated)
	class AOC2Character* ChefPtr = nullptr;

protected:
	UPROPERTY(Replicated)
	float Timer = 0.0f;

	UPROPERTY(Replicated)
	bool bTimerActivated = false;
	
	UPROPERTY(Replicated)
	bool bWashingDone = false;

	UPROPERTY(Replicated)
	float Ratio = 0.0f;

	UPROPERTY(Replicated)
	class UWidgetComponent* ProgressBarComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Table", meta = (AllowprivateAccess = "true"))
	TSubclassOf<UUserWidget> TSubClassWidget = nullptr;

	class UGaugeTextureWidget* WidgetPtr = nullptr;

};

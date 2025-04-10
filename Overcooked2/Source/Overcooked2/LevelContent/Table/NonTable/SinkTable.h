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

	void UpdateProgressBar(float DeltaTime);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void PlacePlates(ACooking* ReceivedCooking);
	void PlacePlates_Implementation(ACooking* ReceivedCooking);

	void DoTheDishes(class AOC2Character* ChefActor);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void WashingIsDone();
	void WashingIsDone_Implementation();
	
	void CheckChefIsWashing();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void HideProgressBar(bool Value);
	void HideProgressBar_Implementation(bool Value);

	//UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void SetPlateVisibility(int Index);
	//void SetPlateVisibility_Implementation(int Index);

	//UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void SetAllPlateHidden();
	//void SetAllPlateHidden_Implementation();
	
	// Æ¯Á¤ ÀÎµ¦½º¸¸, ÄÑ°í ²ö´Ù
	//UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void SetPlateVisibilityWithIndex(int Index, bool Value);
	//void SetPlateVisibilityWithIndex_Implementation(int Index, bool Value);

	void InitProgressBar();

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

	int DirtyPlateNum = 0;

	int CleanPlateNum = 0;

	//UPROPERTY(Replicated)
	//bool KeepWashing = false;

	UPROPERTY(Replicated)
	class UWidgetComponent* ProgressBarComponent = nullptr;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Table", meta = (AllowprivateAccess = "true"))
	UStaticMeshComponent* ComponentForDishes1 = nullptr;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Table", meta = (AllowprivateAccess = "true"))
	UStaticMeshComponent* ComponentForDishes2 = nullptr;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Table", meta = (AllowprivateAccess = "true"))
	UStaticMeshComponent* ComponentForDishes3 = nullptr;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Table", meta = (AllowprivateAccess = "true"))
	UStaticMeshComponent* ComponentForDishes4 = nullptr;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Table", meta = (AllowPrivateAccess = "true"))
	TArray<UStaticMeshComponent*> DirtyPlateComponents;
																	   
	UPROPERTY(/*Replicated, */EditAnywhere, BlueprintReadOnly, Category = "Table", meta = (AllowPrivateAccess = "true"))
	TArray<class APlate*> DirtyPlates;

	UPROPERTY(/*Replicated,*/ EditAnywhere, BlueprintReadOnly, Category = "Table", meta = (AllowPrivateAccess = "true"))
	TArray<class APlate*> CleanPlates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Table", meta = (AllowprivateAccess = "true"))
	TSubclassOf<UUserWidget> TSubClassWidget = nullptr;

	class UGaugeTextureWidget* WidgetPtr = nullptr;


	class UTimeEventComponent* TimeEventComponent = nullptr;
};

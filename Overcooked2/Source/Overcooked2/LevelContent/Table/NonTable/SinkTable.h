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

	//UFUNCTION(Reliable, Server)
	void UpdateProgressBar(float DeltaTime);
	//void UpdateProgressBar_Implementation(float DeltaTime);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void PlacePlates(ACooking* ReceivedCooking);
	void PlacePlates_Implementation(ACooking* ReceivedCooking);

	void DoTheDishes(class AOC2Character* ChefActor);

	UFUNCTION(Server, Reliable)
	void WashingIsDone();
	void WashingIsDone_Implementation();
	
	UFUNCTION(Server, Reliable)
	void CheckChefIsWashing();
	void CheckChefIsWashing_Implementation();

	UFUNCTION(Server, Reliable)
	void FinishWashing();
	void FinishWashing_Implementation();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void HideProgressBar(bool Value);
	void HideProgressBar_Implementation(bool Value);

	UFUNCTION(Server, Reliable)
	void RepeatWashing();
	void RepeatWashing_Implementation();

	//UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	//void SetPlateVisibility(int Index);
	//void SetPlateVisibility_Implementation(int Index);

	//void SetAllPlateVisibility();

	void InitProgressBar();
	
	//UFUNCTION(Reliable, Server)
	//void AddDirtyPlateNum(int Value);
	//void AddDirtyPlateNum_Implementation(int Value);

	//UFUNCTION(Reliable, Server)
	//void AddCleanPlateNum(int Value);
	//void AddCleanPlateNum_Implementation(int Value);

	//UFUNCTION(Reliable, NetMulticast)
	//void SetCleanPlateMesh();
	//void SetCleanPlateMesh_Implementation();

	//void InitDirtyPlateMesh();
	//void InitCleanPlateMesh();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking")
	USceneComponent* ComponentForProgressBar = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CleanPlate")
	USceneComponent* CleanPlateComponent = nullptr;

	UPROPERTY(Replicated)
	class AOC2Character* ChefPtr = nullptr;

protected:
	//UFUNCTION()
	//void OnRep_SetCleanPlateMesh();
	
	//UFUNCTION()
	//void OnRep_SetDirtyPlateMesh();

	void SetAttachToDirtyPlate();

	UFUNCTION()
	void OnRep_ChangeProgress();


	UPROPERTY(Replicated)
	float Timer = 0.0f;

	UPROPERTY(Replicated)
	bool bTimerActivated = false;
	
	UPROPERTY(Replicated)
	bool bWashingDone = false;

	UPROPERTY(ReplicatedUsing = OnRep_ChangeProgress)
	float Ratio = 0.0f;

	//UPROPERTY(ReplicatedUsing = OnRep_SetDirtyPlateMesh)
	//int DirtyPlateNum = 0;

	//UPROPERTY(ReplicatedUsing = OnRep_SetCleanPlateMesh)
	//int CleanPlateNum = 0;

	UPROPERTY(Replicated)
	class UWidgetComponent* ProgressBarComponent = nullptr;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Table", meta = (AllowprivateAccess = "true"))
	USceneComponent* ComponentForDishes1 = nullptr;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Table", meta = (AllowprivateAccess = "true"))
	USceneComponent* ComponentForDishes2 = nullptr;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Table", meta = (AllowprivateAccess = "true"))
	USceneComponent* ComponentForDishes3 = nullptr;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Table", meta = (AllowprivateAccess = "true"))
	USceneComponent* ComponentForDishes4 = nullptr;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Table", meta = (AllowPrivateAccess = "true"))
	TArray<USceneComponent*> DirtyPlateComponents;

	//UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Table", meta = (AllowPrivateAccess = "true"))
	//UStaticMeshComponent* CleanPlateMeshComponent = nullptr;
																	  
	TArray<APlate*> DirtyPlates;
	TArray<APlate*> CleanPlates;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Table", meta = (AllowprivateAccess = "true"))
	TSubclassOf<UUserWidget> TSubClassWidget = nullptr;

	class UGaugeTextureWidget* WidgetPtr = nullptr;

	class UTimeEventComponent* TimeEventComponent = nullptr;

	FVector PlateSize = { 2.0f, 2.0f, 2.0f};

};

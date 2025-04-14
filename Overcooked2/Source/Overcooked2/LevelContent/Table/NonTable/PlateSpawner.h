// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelContent/Table/CookingTable.h"
#include "PlateSpawner.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API APlateSpawner : public ACookingTable
{
	GENERATED_BODY()

public:
	APlateSpawner();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void SetPlate(class APlate* Plate);

	virtual ACooking* Interact(AActor* ChefActor) override;

	void PlaceItem(ACooking* ReceivedCooking) override;

	UFUNCTION(Reliable, NetMulticast)
	void GetOwnedPlate();
	void GetOwnedPlate_Implementation();

	UFUNCTION(Reliable, NetMulticast)
	void MoveToServer(APlate* Plate);
	void MoveToServer_Implementation(APlate* Plate);

	UFUNCTION(Reliable, NetMulticast)
	void SetPlateMesh();
	void SetPlateMesh_Implementation();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(Reliable, NetMulticast)
	void AddPlate(int Number);
	void AddPlate_Implementation(int Number);

	UFUNCTION(Reliable, NetMulticast)
	void InitPlateNum();
	void InitPlateNum_Implementation();

	UFUNCTION()
	void OnRep_SetPlateMesh();

private:

	//int PlateNum = 0;

	//TMap<int, ACooking*> PlateMap;

	class APlate* OwnedPlate;
	ACooking* NewCooking = nullptr;
	bool bOwnedPlate = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PlateMeshComponent = nullptr;

	UPROPERTY(ReplicatedUsing = OnRep_SetPlateMesh, EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	int PlateNum = 0;

	USceneComponent* SceneComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	UTexture* DirtyTexture = nullptr;
};

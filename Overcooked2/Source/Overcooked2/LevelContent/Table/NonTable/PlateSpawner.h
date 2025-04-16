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

	void SetPlate(class APlate* Plate);

	virtual ACooking* Interact(AActor* ChefActor) override;

	void PlaceItem(ACooking* ReceivedCooking) override;

private:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void SetPlateMesh();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(Reliable, Server)
	void AddPlate(int Number);
	void AddPlate_Implementation(int Number);

	UFUNCTION(Reliable, Server)
	void InitPlateNum();
	void InitPlateNum_Implementation();

	UFUNCTION()
	void OnRep_SetPlateMesh();

	UFUNCTION(NetMulticast, Reliable)
	void PlaySoundEffect();
	void PlaySoundEffect_Implementation();

private:

	void SetMaterialTextrue();

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PlateMeshComponent = nullptr;

	UPROPERTY(ReplicatedUsing = OnRep_SetPlateMesh, EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	int PlateNum = 0;

	USceneComponent* SceneComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	UTexture* DirtyTexture = nullptr;

	APlate* CookingPtrPlate = nullptr;

	USoundBase* SoundEffect = nullptr;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelContent/Table/CookingTable.h"
#include "OC2CharacterTestTable.generated.h"

UCLASS()
class OVERCOOKED2_API AOC2CharacterTestTable : public ACookingTable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOC2CharacterTestTable();

	virtual ACooking* Interact(AActor* ChefActor) override;
	AIngredient* SpawnIngredient(AActor* ChefActor);
	virtual void PlaceItem(ACooking* Cook);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ACooking* CurCook = nullptr;

	void Highlight();
	void OffHighlight();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* MySceneComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UMaterial* OriginMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UMaterial* HighlightMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	EIngredientType IngredientType;
};

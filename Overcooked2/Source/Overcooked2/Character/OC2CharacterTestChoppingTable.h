// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelContent/Table/CookingTable.h"
#include "OC2CharacterTestChoppingTable.generated.h"

UCLASS()
class OVERCOOKED2_API AOC2CharacterTestChoppingTable : public ACookingTable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOC2CharacterTestChoppingTable();

	virtual ACooking* Interact(AActor* ChefActor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private:
	ACooking* CurCook;
	virtual void PlaceItem(ACooking* Cook);

	UPROPERTY(VisibleAnywhere)
	bool IsChopping;

	float ChopDuration = 3.0f;
	float ChopTime = 0.0f;
};

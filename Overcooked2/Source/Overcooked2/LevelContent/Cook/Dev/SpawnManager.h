// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Global/OC2Enum.h>
#include "SpawnManager.generated.h"

UCLASS()
class OVERCOOKED2_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnManager();

	UFUNCTION(BlueprintCallable)
	void SetIngredientType(UWorld* World, EIngredientType Type);

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void RequestSpawnIngredient(UWorld* World);
	void RequestSpawnIngredient_Implementation(UWorld* World);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
private:	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	EIngredientType IngredientType = EIngredientType::EIT_NONE;
};

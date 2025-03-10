// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include <Global/OC2Enum.h>
#include "CookingDevGameState.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ACookingDevGameState : public AGameState
{
	GENERATED_BODY()

public:
	ACookingDevGameState();

	UFUNCTION(BlueprintCallable)
	void SetIngredientType(EIngredientType Type);

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void RequestSpawnIngredient();
	void RequestSpawnIngredient_Implementation();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	EIngredientType IngredientType = EIngredientType::EIT_NONE;
};

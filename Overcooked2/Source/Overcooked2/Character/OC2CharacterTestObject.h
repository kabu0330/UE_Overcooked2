// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelContent/Cook/Cooking.h"
#include "OC2CharacterTestObject.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API AOC2CharacterTestObject : public ACooking
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AOC2CharacterTestObject();

	UFUNCTION(BlueprintCallable)
	void SetIngredient(EIngredientType Type);

	void SetPhysics(bool Value, AActor* Player);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private:
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	EIngredientType IngredientType = EIngredientType::EIT_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	EIngredientState CurIngredientState = EIngredientState::EIS_NONE;


};

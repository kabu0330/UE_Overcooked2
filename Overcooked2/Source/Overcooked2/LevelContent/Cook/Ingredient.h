// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <LevelContent/Cook/Cooking.h>
#include "Ingredient.generated.h"

// 요리 재료
UCLASS()
class OVERCOOKED2_API AIngredient : public ACooking/*, public UInterActable*/
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIngredient();

	// 상자에서 꺼냈을 때
	UFUNCTION(BlueprintCallable)
	void Init(FName Name);
	
	UFUNCTION(BlueprintCallable)
	const FIngredientCookDataRow& CheckState(EIngredientState State);

	UFUNCTION(BlueprintCallable)
	void ChageState(EIngredientState State);


	// 도마에서 썰어야 하는 재료야?
	UFUNCTION(BlueprintCallable)
	bool IsChoppable() const
	{
		return CurIngredientState == EIngredientState::EIS_CHOPPABLE;
	}

	// 프라이팬에 구워야 하는 재료야?
	UFUNCTION(BlueprintCallable)
	bool IsGrillable() const
	{
		return CurIngredientState == EIngredientState::EIS_GRILLABLE;
	}

	// 솥에서 밥을 지을 수 있어?
	UFUNCTION(BlueprintCallable)
	bool IsBoilable() const
	{
		return CurIngredientState == EIngredientState::EIS_BOILABLE;
	}

	UFUNCTION(BlueprintCallable)
	EIngredientType GetIngredientType() const 
	{
		return IngredientType;
	}

	UFUNCTION(BlueprintCallable)
	EIngredientState GetCurIngredientState() const
	{
		return CurIngredientState;
	}

	UFUNCTION(BlueprintCallable)
	const FIngredientDataRow&  GetDataTable() const
	{
		return DataTable;
	}

	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* const GetStaticMeshComponent() const
	{
		return StaticMeshComponent;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	FIngredientDataRow DataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	EIngredientType IngredientType = EIngredientType::EIT_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	EIngredientState CurIngredientState = EIngredientState::EIS_NONE;
};

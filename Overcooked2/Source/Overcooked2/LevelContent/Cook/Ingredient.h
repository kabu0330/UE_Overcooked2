// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <LevelContent/Cook/Cooking.h>
#include <Global/OC2Enum.h>
//#include <Global/Interface/InterActable.h>
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
	
	//UFUNCTION(BlueprintCallable)
	//const FIngredientCookDataRow* CheckState(EIngredientState IngredientState);

	UFUNCTION(BlueprintCallable)
	void ChageState(EIngredientState State);


	// 조리가 끝난 재료야?
	UFUNCTION(BlueprintCallable)
	bool IsCooked() const 
	{
		return IngredientState == EIngredientState::EFS_COOKED;
	}

	// 도마에서 썰어야 하는 재료야?
	UFUNCTION(BlueprintCallable)
	bool IsChoppable() const
	{
		return IngredientState == EIngredientState::EFS_CHOPPABLE;
	}

	// 프라이팬에 구워야 하는 재료야?
	UFUNCTION(BlueprintCallable)
	bool IsGrillable() const
	{
		return IngredientState == EIngredientState::EFS_GRILLABLE;
	}

	// 솥에서 밥을 지을 수 있어?
	UFUNCTION(BlueprintCallable)
	bool IsBoilable() const
	{
		return IngredientState == EIngredientState::EFS_BOILABLE;
	}

	UFUNCTION(BlueprintCallable)
	EIngredientType GetIngredientType() const 
	{
		return IngredientType;
	}

	UFUNCTION(BlueprintCallable)
	EIngredientState GetIngredientState() const
	{
		return IngredientState;
	}

	//FIngredientDataRow* const GetDataTable() const
	//{
	//	return DataTable;
	//}

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
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	//const FIngredientDataRow* DataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	EIngredientType IngredientType = EIngredientType::EFD_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	EIngredientState IngredientState = EIngredientState::EFS_NONE;
};

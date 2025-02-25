// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include <Global/Interface/InterActable.h>
#include "Ingredient.generated.h"

// 요리 재료
UCLASS()
class OVERCOOKED2_API AIngredient : public AActor, /*public UInterActable*/
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIngredient();

	// 생성자에서 기본적인 데이터를 입력하게 해야하나?
	
	// 조리가 끝난 재료야?
	UFUNCTION(BlueprintCallable)
	bool IsCooked() const 
	{
		//return IngredientState == EIngredientState::COOKED;
		return false;
	}

	// 도마에서 썰어야 하는 재료야?
	UFUNCTION(BlueprintCallable)
	bool IsChoppable() const
	{
		//return IngredientState == EIngredientState::CHOPPABLE;
		return false;
	}

	// 프라이팬에 구워야 하는 재료야?
	UFUNCTION(BlueprintCallable)
	bool IsGrillable() const
	{
		//return IngredientState == EIngredientState::GRILLABLE;
		return false;
	}

	// 솥에서 밥을 지을 수 있어?
	UFUNCTION(BlueprintCallable)
	bool IsBoilable() const
	{
		//return IngredientState == EIngredientState::BOILABLE;
		return false;
	}


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cook", meta = (AllowPrivateAccess = "true"))
	FString Name = TEXT("");

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cook", meta = (AllowPrivateAccess = "true"))
	//EIngredientState IngredientState = EIngredientState::NONE;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cook", meta = (AllowPrivateAccess = "true"))
	//EIngredientType IngredientType = EIngredientType::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cook", meta = (AllowPrivateAccess = "true"))
	float CookingTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cook", meta = (AllowPrivateAccess = "true"))
	float OvercookTime = 0.0f;

};

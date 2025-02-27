// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <LevelContent/Cook/Cooking.h>
#include <Global/OC2Enum.h>

#include "Plate.generated.h"

// 접시 ~ 접시에 올라간 조리된 요리 ~ 요리들의 조합 ~ 완성된 요리
UCLASS()
class OVERCOOKED2_API APlate : public ACooking
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlate();


	UFUNCTION(BlueprintCallable)
	void CleanPlate();

	// 접시 위에 재료를 쌓는 모든 과정
	UFUNCTION(BlueprintCallable)
	bool Add(class AIngredient* Ingredient);


	void CookCheck();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	const FStageCookingDataRow* Data;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	TArray<class AIngredient*> Ingredients;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* IngredientMesh = nullptr; // 재료

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PlateMesh = nullptr; // 접시

	

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrderManageComponent.generated.h"

//class AIngredient;

USTRUCT(BlueprintType)
struct OVERCOOKED2_API FRecipe
{
	GENERATED_BODY()

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Order")
	//TArray<TObjectPtr<AIngredient>> RequiredIngredients;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Order")
	TArray<UClass*> RequiredActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Order")
	float TimeLimit = 0.0f;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVERCOOKED2_API UOrderManageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOrderManageComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TArray<FRecipe> OrderList;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Global/OC2Const.h"
#include "Global/OC2Enum.h"
#include "Global/OC2Struct.h"

#include "OrderManageComponent.generated.h"

//class AIngredient;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVERCOOKED2_API UOrderManageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UOrderManageComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CreateNewOrder(FOrder Order);

private:
	//TArray<FRecipe> OrderList;
};

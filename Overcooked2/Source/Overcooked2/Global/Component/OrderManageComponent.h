// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Global/OC2Const.h"
#include "Global/OC2Enum.h"
#include "Global/OC2Struct.h"

#include "OrderManageComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVERCOOKED2_API UOrderManageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UOrderManageComponent();

private:
	int FindOrderIndex(FOrder& Order);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CreateNewOrder(FOrder Order);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CompleteOrder(FOrder Order);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BlinkOrderUI();

private:
	TArray<FOrder> OrderList;
};

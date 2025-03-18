// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Global/OC2Enum.h>
#include <Global/Data/IngredientDataTable.h>
#include <Global/OC2GameInstance.h>
#include <Global/GameFramework/OC2Actor.h>
#include <Global/Interface/Interactable.h>
#include "Cooking.generated.h"

// Ingredient(요리 재료)와 Plate(요리)의 최상위 클래스
// 캐릭터가 Grab할 수 있는 모든 맵 오브젝트들의 최상위 클래스
UCLASS()
class OVERCOOKED2_API ACooking : public AOC2Actor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACooking();

	// Interact
	UFUNCTION(NetMulticast, Reliable)
	void AttachToChef(AActor* Player);
	void AttachToChef_Implementation(AActor* Player);

	UFUNCTION(NetMulticast, Reliable)
	void DetachFromChef(AActor* Player);
	void DetachFromChef_Implementation(AActor* Player);

	virtual void DetachAction() {};


	const FIngredientDataRow* const GetIngredientDataTable() const
	{
		return IngredientDataTable;
	}
	
	UFUNCTION(BlueprintCallable)
	void SetSimulatePhysics(bool Value)
	{
		StaticMeshComponent->SetSimulatePhysics(Value);
	}


	UFUNCTION(BlueprintCallable)
	const ECookingType GetCookingType() const 
	{
		return CookingType;
	}

	UFUNCTION(BlueprintCallable)
	bool IsCookingType(ECookingType Type) const
	{
		return Type == CookingType;
	}

	UFUNCTION(NetMulticast, Reliable)
	void SetCookingTable(class ACookingTable* Table);
	void SetCookingTable_Implementation(class ACookingTable* Table);

	// 템플릿은 BP에서 호출할 수 없다.
	template<typename T>
	T* GetChild(T* Cooking)
	{
		T* NewCooking = Cast<T>(Cooking);
		return NewCooking;
	}


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ForwardCookingTable(class ACookingTable* Table) {};
	virtual void ForwardAttachToChef() {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	ECookingType CookingType = ECookingType::ECT_NONE;

	const FIngredientDataRow* IngredientDataTable = nullptr;

private:	
};

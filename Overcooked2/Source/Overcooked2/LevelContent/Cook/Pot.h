// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelContent/Cook/Cooking.h"
#include "Pot.generated.h"

UENUM(BlueprintType)
enum class EPotState : uint8
{
	IDLE 		UMETA(DisplayName = "대기"),
	HEATING		UMETA(DisplayName = "가열 중"),
	BOILING		UMETA(DisplayName = "조리 중"),
	COOKED		UMETA(DisplayName = "조리 완료"),
	OVERCOOKED	UMETA(DisplayName = "태움"),
	MAX			UMETA(DisplayName = "MAX"),
};

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API APot : public ACooking
{
	GENERATED_BODY()
	
public:
	APot();

	// 처음 재료가 들어왔을 때 호출할 함수
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Add(class AIngredient* Ingredient);
	void Add_Implementation(class AIngredient* Ingredient);

	UFUNCTION(BlueprintCallable)
	void SetBoil(ACooking* Rice);

	// 조리된 밥을 받아오는 함수
	UFUNCTION(BlueprintCallable)
	class AIngredient* GetCookedIngredient();



protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Cook(float DeltaTime);

	void ChangeAnimation();
	void ChangeMaterialColor();


	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	virtual void ForwardCookingTable(class ACookingTable* Table) override;
	virtual void ForwardAttachToChef() override;

private:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	USkeletalMeshComponent* SoupSkeletalMeshComponent = nullptr;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	TArray<UMaterialInstanceDynamic*> SoupDynamicMaterial;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	EPotState PotState = EPotState::IDLE;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	EPotState PrevPotState = EPotState::MAX;

	UPROPERTY(Replicated)
	bool bIsRiceInPot = false;
	
	float TimeElapsed = 0.0f;

	UPROPERTY(Replicated)
	class ACookingTable* CookingTable = nullptr;

	UPROPERTY(Replicated)
	bool bIsBoiling = false;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelContent/Cook/Cooking.h"
#include "Pot.generated.h"

UENUM(BlueprintType)
enum class EPotState : uint8
{
	IDLE 		UMETA(DisplayName = "대기"),
	HEATING		UMETA(DisplayName = "HEATING"),
	BOILING		UMETA(DisplayName = "BOILING"),
	COOKED		UMETA(DisplayName = "COOKED"),
	OVERCOOKED	UMETA(DisplayName = "OVERCOOKED"),
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

	// 조리된 밥을 받아오는 함수
	// 캐릭터가 RPC Server 함수로 호출 요청
	UFUNCTION(BlueprintCallable)
	class AIngredient* GetRice();

	EPotState GetPotState() const
	{
		return PotState;
	}

	void ResetPot();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UMaterialInstanceDynamic* LoadNoneMaterial();
	void Cook(float DeltaTime);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void SetAction();
	void SetAction_Implementation();

	void ChangeMaterialColor(FVector4 Color);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void SetSoupMaterial();
	void SetSoupMaterial_Implementation();

	bool IsBoiling();


	void ChangeNoneMaterial();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	virtual void ForwardCookingTable(class ACookingTable* Table) override;
	virtual void ForwardAttachToChef() override;

	void InitTexture();
	void SetWarningTexture();
	void SetWarnigTextureOffset();

private:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	USkeletalMeshComponent* SoupSkeletalMeshComponent = nullptr;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	TArray<UMaterialInstanceDynamic*> SoupDynamicMaterial;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	UMaterialInstanceDynamic* NoneMaterial = nullptr;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	EPotState PotState = EPotState::IDLE;

	UPROPERTY(/*Replicated,*/ EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	EPotState PrevPotState = EPotState::MAX;

	UPROPERTY(Replicated)
	bool bIsRiceInPot = false;
	
	UPROPERTY(Replicated)
	float TimeElapsed = 0.0f;

	UPROPERTY(Replicated)
	class ACookingTable* CookingTable = nullptr;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	class UBillboardComponent* TextureBillboard = nullptr;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <LevelContent/Cook/Cooking.h>
#include <Global/OC2Enum.h>
#include <Global/OC2Const.h>
#include "Plate.generated.h"

UENUM(BlueprintType)
enum class EPlateStackStatus : uint8
{
	SINGLE		UMETA(DisplayName = "1개"),
	DOUBLE		UMETA(DisplayName = "2개"),
	TRIPLE		UMETA(DisplayName = "3개"),
	FULL		UMETA(DisplayName = "4개")
};

class APlateSpawner;

// 접시 ~ 접시에 올라간 조리된 요리 ~ 요리들의 조합 ~ 완성된 요리
UCLASS()
class OVERCOOKED2_API APlate : public ACooking
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlate();

	// 접시 위에 재료를 쌓는 모든 과정
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Add(class AIngredient* Ingredient);
	void Add_Implementation(class AIngredient* Ingredient);

	bool IsCombinationSuccessful() const
	{
		return bIsCombinationSuccessful;
	}


	UFUNCTION(BlueprintCallable)
	bool IsDirtyPlate();

	// 서버(GameMode)에서 호출해야만 적용되는 함수들!!!
	// 설거지 성공 시 호출
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void WashPlate();
	void WashPlate_Implementation();

	// 접시의 상태를 Dirty로 바꿔서 스폰하고 싶으면 호출
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void SetPlateState(EPlateState State);
	void SetPlateState_Implementation(EPlateState State);

	// 접시 위에 요리 재료를 버리고 접시를 초기 상태로 되돌림
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void CleanPlate();
	void CleanPlate_Implementation();

	// 접시를 쌓기 위해 호출해야 할 함수 (메시 변환 자동)
	UFUNCTION(BlueprintCallable/*, NetMulticast, Reliable*/)
	void StackPlate(APlate* Plate);
	/*void StackPlate_Implementation(APlate* Plate);*/

	TArray<FRecipe> GetIngredients() const
	{
		return Ingredients;
	}

	FRecipe GetIngredient(int32 Index) const
	{
		return Ingredients[Index];
	}
	UFUNCTION(NetMulticast, Reliable)
	void SubmitPlate();

	void SpawnPlate();

	TArray<APlate*>& GetAnotherPlatesRef()
	{
		return AnotherPlates;
	}

	// 깨끗한 접시 중 하나를 꺼낸다.
	APlate* TakeCleanPlate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	bool CanPlaceOnPlate(AIngredient* Ingredient);
	void SetIngredinetMesh(FPlateInitData Data);
	void SetIngredinetTextures(FPlateInitData Data);

	void SetMesh();
	void SetMaterialTexture(UTexture* Texture);

	virtual void ForwardAttachToChef() override;
	virtual void ForwardDetachToChef() override;

	virtual void PostInitializeComponents() override;

	void ChangePlateMesh();
	void HideAnotherPlates();

	void InitWidgetComponent();
	void FindPlateSpawner();

	void StackUpPlate(EPlateStackStatus Status, FName Name);

private:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	TArray<FRecipe> Ingredients;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* IngredientMesh = nullptr; // 재료

	// 접시 상태
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	EPlateState PlateState = EPlateState::EMPTY;

	// 접시가 쌓인 상태
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	EPlateStackStatus PlateStackStatus = EPlateStackStatus::SINGLE;

	// 나 말고 다른 접시가 나한테 쌓였다면 다른 녀석들의 포인터를 가지고 있을 것임
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	TArray<APlate*> AnotherPlates;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	UTexture* CleanTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	UTexture* DirtyTexture = nullptr;

	UPROPERTY(Replicated)
	bool bIsCombinationSuccessful = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	class UWidgetComponent* WidgetComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	TSubclassOf<UUserWidget> SubclassWidget = nullptr; // 에디터에서 가져올 WBP 지정

	UPROPERTY()
	class UPlateIconWidget* IconWidget = nullptr; // 세팅한 위젯 객체 저장 및 함수 호출용

	UPROPERTY()
	APlateSpawner* PlateSpawner = nullptr;

};

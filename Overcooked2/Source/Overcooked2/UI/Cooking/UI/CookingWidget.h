// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CookingWidget.generated.h"

/**
 * 
 */
USTRUCT()
struct FOrder
{
	GENERATED_BODY()

	class UImage* BackgroundImage = nullptr;
	bool bIsOrder = false;
};


UCLASS()
class OVERCOOKED2_API UCookingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "OCUI")
	void SetCompleteOrderNum(int num)
	{
		CompleteOrderNum = num;
	}

	UFUNCTION(BlueprintCallable, Category = "OCUI")
	void OrderComplete();

	UFUNCTION(BlueprintCallable, Category = "OCUI")
	void NewOrder();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OCUI")
	class UImage* OrderBackground_0;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OCUI")
	class UImage* OrderBackground_1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OCUI")
	class UImage* OrderBackground_2;



protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


private:
	int CompleteOrderNum = 0;
	int NewOrderNum = 0;

	TArray<FOrder> Orders;

	FTimerHandle OpacityTimerHandle;
	float OpacityOffset = 0.05f;

	FTimerHandle MoveTimerHandle;
	FVector2D TargetOffset = FVector2D(50.0f, 0.0f);
	float MoveTimeElapsed = 10.0f;

	float ImageOffset = 10.0f;
	float FinalPos = 0.0f;
	int Size = 3;

	void UpdateImageOpacity();
	void UpdateImagePosition();
	void MoveNewOrder();

};

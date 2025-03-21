// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CookingTimeWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UCookingTimeWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UTextBlock* Time = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UProgressBar* TimeProgressBar = nullptr;

	UFUNCTION(BlueprintCallable, Category = "OC2UI")
	void SetbIsStart(bool Value)
	{
		bIsStart = Value;
	}

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;


private:
	void StartTimer(float Deltatime);

	bool bIsStart = false;
	float TotalTime = 180.0f;
	float CurTime = 0.0f;



};

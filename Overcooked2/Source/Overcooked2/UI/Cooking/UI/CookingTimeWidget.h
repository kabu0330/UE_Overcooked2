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



	bool GetIsTimesUP()
	{
		return bIsTimesUP;
	}

	void SetIsTimesUP(bool IsStart)
	{
		bIsTimesUP = IsStart;
	}

	void SetStartTimer(bool IsStart);
	float StartTimerTick(float DeltaTime);


protected:
	//virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
	virtual void NativeConstruct() override;


private:
	float StartTimer(float Deltatime);

	bool bIsStart = false;
	bool bIsTimesUP = false;
	float TotalTime = 10.0f;
	float CurTime = 0.0f;


};

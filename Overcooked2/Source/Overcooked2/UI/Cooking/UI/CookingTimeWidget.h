// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TimelineComponent.h"
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

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UImage* TimeImg = nullptr;



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
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OC2UI")
	class UCurveFloat* TimerCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OC2UI")
	class UCurveFloat* TimerTextCurve = nullptr;

private:
	float StartTimer(float Deltatime);

	bool bIsStart = false;
	bool bIsTimesUP = false;
	float TotalTime = 180.0f;
	float CurTime = 0.0f;
	int OriginalFontSize = 0;

	UFUNCTION()
	void PlayTimerAnimation(float Value);

	UFUNCTION()
	void PlayTimerTextAnimation(float Value);

	void UpdateTimerScale(FLinearColor Color, float FontSize, FVector2D Position);

	void UpdateTimerAngle(float Angle);

	FTimeline TimerImgTimeline;
	FTimeline TimerTextTimeline;

};

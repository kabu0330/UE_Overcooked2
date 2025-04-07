// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TimelineComponent.h"
#include "CookingScoreWidget.generated.h"

/**
 *
 */
UCLASS()
class OVERCOOKED2_API UCookingScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void PlayOderCompleteAnimation(int Score);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UTextBlock* ScoreText = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UTextBlock* AddText = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UImage* CoinFlameImage = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UTextBlock* TipText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OC2UI")
	UCurveFloat* ScoreCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OC2UI")
	UCurveFloat* AddScoreCurve = nullptr;

	UFUNCTION(BlueprintCallable, Category = "OC2UI")
	void PlayCoinFlameAnimation();


	UFUNCTION(BlueprintCallable, Category = "OC2UI")
	void StopCoinFlameAnimation();


	void SetTipCount(int TipCount);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UImage* CoinBannerBar = nullptr;


protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

private:
	void PlayCoinAnimation();


	UFUNCTION()
	void PlayScoreAnimation(float Value);

	UFUNCTION()
	void PlayAddScoreAnimation(float Value);

	void UpdateScore(FLinearColor Color, int Size, FVector2D Position);
	class UMaterialInstanceDynamic* CoinMaterial = nullptr;
	class UMaterialInstanceDynamic* CoinFlameMaterial = nullptr;

	FTimerHandle AnimationTimer;
	FTimerHandle FlameAnimationTimer;

	FTimeline ScoreTimeline;
	FTimeline AddScoreTimeline;

	void ResetScaleAndColor();
	void ResetLocation();

	int OriginalFontSize = 0.0;

	int TotalScore = 0;
	int CurScore = 0;

	//test
	bool bIsFlamePlay = false;
};

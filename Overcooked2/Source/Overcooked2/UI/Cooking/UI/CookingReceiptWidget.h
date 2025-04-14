// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TimelineComponent.h"
#include "CookingReceiptWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UCookingReceiptWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "OC2UI")
	void CheckStar();

	UFUNCTION(BlueprintCallable, Category = "OC2UI")
	int GetTotalStart()
	{
		return TotalStar;
	}

	UFUNCTION(BlueprintCallable, Category = "OC2UI")
	void ShowScoreText();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UCanvasPanel* ReceiptCanvas = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UCanvasPanel* TxtCanvas = nullptr;
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OC2UI")
	class UCurveFloat* StarCurve = nullptr;

private:

	UFUNCTION()
	void PlayStarAnimaion(float Value);

	UFUNCTION()
	void ResetSize();

	int CurIndex = 0;

	void UpdateStar(int Count, FVector2D Size);
	void PlayScoreTextAnimation();


	class UImage* CurImg = nullptr;
	FTimeline StarTimeline;

	int CurTime = 0;
	FTimerHandle ScoreTextTimerHandle;

	int TotalStartScore = 0;
	int TotalStar = 0;

	int FistStartScore = 40;
	int SecondStartScore = 120;
	int ThirdStartScore = 160;

	template <typename T>
	T* FindChildWidget(const FString& name, UCanvasPanel* canvas);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TimelineComponent.h"
#include "TitleStartWidget.generated.h"

/**
 * 
 */

UENUM()
enum class ETitleAnim : uint8
{
	None,
	OvercookedFadeIn,
	GhostFadeIn,
	Team17FadeIn,
	UEFadeIn,
	FadeOut
};


UCLASS()
class OVERCOOKED2_API UTitleStartWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UImage* Background = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UImage* StartImg = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UImage* UEImage = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UImage* T17Img = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UImage* GhostImg = nullptr;

	UFUNCTION(BlueprintCallable, Category = "OC2UI")
	void SkipAnimation();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UCanvasPanel* StartWidget = nullptr;

	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
	virtual FReply NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OC2UI")
	class UCurveFloat* ImgCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OC2UI")
	class UCurveFloat* FadeOutCurve = nullptr;

private:

	void SetGhostTownAnimation();

	UFUNCTION()
	void StartTitleAnimation(float Value);

	UFUNCTION()
	void EndTitleAnimation();

	UFUNCTION()
	void PlayTeam17Animation();

	UFUNCTION()
	void PlayUEAnimation();

	UFUNCTION()
	void SetVisibleCollapsed();


	void UpdateImageOpacity(float CurOpacity);


	FTimeline ImgTimeline;
	ETitleAnim CurrentPhase = ETitleAnim::None;

	class UImage* CurImg = nullptr;
	bool bIsUE = false;
	bool bIsGhost = false;
	bool bIsT17 = false;
	float StartAnimTime = 0.0f;

};

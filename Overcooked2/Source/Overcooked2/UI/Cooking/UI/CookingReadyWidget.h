// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CookingReadyWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UCookingReadyWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UImage* ReadyProgress = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UImage* SpaceBarImg = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UTextBlock* SkipText = nullptr;
	

	bool bIsReady = false;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UImage* TransitionImg = nullptr;

protected:
	void NativeConstruct();
	void NativeTick(const FGeometry& MyGeometry, float DeltaTime);
	
	
private:
	void PlayZoomOutAnimation();
	class UMaterialInstanceDynamic* ProgressMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OC2UI", meta = (AllowPrivateAccess = "true"))
	bool bHoldingSpace = false;


	float ProgressTime = 0.f;
	float HoldSpeed = 1.f; // 초당 1.0까지 (1초에 가득참)

	class UMaterialInstanceDynamic* TransitionMaterial = nullptr;
	FTimerHandle AnimationTimer;


};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingWidget.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ELevelChangType : uint8
{
	WorldMapToSushi UMETA(DisplayName = "World Map to Sushi Level"),
	LobbyToWorldMap UMETA(DisplayName = "Lobby to World Map")
};

UCLASS()
class OVERCOOKED2_API ULoadingWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetProgress(float Value);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UProgressBar* LoadingProgressBar = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UImage* TransitionImg = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UImage* ConnectingImage = nullptr;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UCanvasPanel* ConnectingCanvas = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UCanvasPanel* WorldMapToSushiLevel = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	class UCanvasPanel* LobbyToWorldMap = nullptr;



	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSoftObjectPtr<UMaterialInterface> BugerMaterialAsset;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSoftObjectPtr<UMaterialInterface> PizzaMaterialAsset;



	void PlayLoadingAnimation(ELevelChangType LevelEnum = ELevelChangType::LobbyToWorldMap);

	void PlayZoomOutAnimation();

	void PlayZoomInAnimation();

	bool GetIsConnecting()
	{
		return bIsConnecting;
	}

	void SetIsConnecting(bool Value)
	{
		bIsConnecting = Value;
	}


protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime);


private:
	class UMaterialInstanceDynamic* TransitionMaterial = nullptr;
	class UMaterialInstanceDynamic* ConnectingMaterial = nullptr;

	FTimerHandle AnimationTimer;
	FTimerHandle LoadingAnimationTimer;

	bool bIsConnecting = false;
	float ProgressTime = 0.0f;
	float BugerAnimationTotalIndex = 0.84f;
	float PizzaAnimationTotalIndex =  0.92f;
	float CurIndex = 0.0f;
	float CurTime = 0.0f;


	ELevelChangType CurPanelType = ELevelChangType::WorldMapToSushi;

	//TFunction<void()> Function;
};


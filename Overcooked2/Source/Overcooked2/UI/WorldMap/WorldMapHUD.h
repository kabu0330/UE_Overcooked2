// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WorldMapHUD.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API AWorldMapHUD : public AHUD
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "OC2UI")
	void ShowLoadingWidget();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category = "OC2UI")
	class UWorldMapUserWidget* WorldMapUserWidget = nullptr;

	UPROPERTY(EditAnywhere, Category = "OC2UI")
	TSubclassOf<class UWorldMapUserWidget> WorldWidgetObject;

	class ULoadingWidget* LoadingWidget = nullptr;

	UPROPERTY(EditAnywhere, Category = "OC2UI")
	TSubclassOf<class ULoadingWidget> LoadingWidgetObject;


	class UWorldMapStageWidget* WorldMapStageWidget = nullptr;

	UPROPERTY(EditAnywhere, Category = "OC2UI")
	TSubclassOf<class UWorldMapStageWidget> WorldMapStageWidgetObject;
};

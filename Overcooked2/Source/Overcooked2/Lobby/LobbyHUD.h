// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyHUD.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ALobbyHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "OC2UI")
	void ShowLoadingWidget();


	UPROPERTY(EditAnywhere, Category = "OC2UI")
	class ULobbyUserWidget* LobbyWidget = nullptr;

	UPROPERTY(EditAnywhere, Category = "OC2UI")
	TSubclassOf<class ULobbyUserWidget> LobbyWidgetObject;

	UPROPERTY(EditAnywhere, Category = "OC2UI")
	class ULobbyZoomInWidget* LobbyZoomInWidget = nullptr;

	UPROPERTY(EditAnywhere, Category = "OC2UI")
	TSubclassOf<class ULobbyZoomInWidget> LobbyZoomInWidgetObject;


	class ULoadingWidget* LoadingWidget = nullptr;

	UPROPERTY(EditAnywhere, Category = "OC2UI")
	TSubclassOf<class ULoadingWidget> LoadingWidgetObject;
};

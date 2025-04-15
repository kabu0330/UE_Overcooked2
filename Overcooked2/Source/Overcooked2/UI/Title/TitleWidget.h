// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TimelineComponent.h"
#include "TitleWidget.generated.h"

/**
 *
 */
UCLASS()
class OVERCOOKED2_API UTitleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Global|Network")
	void StartServer();

	UFUNCTION(BlueprintCallable, Category = "Global|Network")
	void Connect();

	UFUNCTION(BlueprintCallable, Category = "UI|Sound")
	void PlayOpenSound();

	UFUNCTION(BlueprintCallable, Category = "UI|Sound")
	void PlaySelectSound();

	UFUNCTION(BlueprintCallable, Category = "UI|Sound")
	void PlaySubMenuSound();

protected:

	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "OC2UI")
	bool bIsOpen = false;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Network", meta = (AllowPrivateAccess = "true"))
	FString IP = TEXT("127.0.0.1");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Network", meta = (AllowPrivateAccess = "true"))
	FString Port = TEXT("30000");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Network", meta = (AllowPrivateAccess = "true"))
	FString LevelName = TEXT("LobbyLevel");

};
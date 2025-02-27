// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ULobbyUserWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	// Temp
	const FString PLAY_LEVEL = "OC2CharacterDevLevel";

	UFUNCTION(BlueprintCallable, meta=(AllowPrivateAccess="true"))
	void MoveToPlayLevel();
};

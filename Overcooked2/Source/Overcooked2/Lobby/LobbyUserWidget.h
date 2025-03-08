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
	const FString LOBBY_LEVEL = "LobbyDevLevel";
	const FString PLAY_LEVEL = "GlobalDevLevel";

	UFUNCTION(BlueprintCallable, meta=(AllowPrivateAccess="true"))
	void MoveToPlayLevel();

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void StartServer();

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void ConnectServer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Network", meta = (AllowPrivateAccess = "true"))
	FString IP = TEXT("127.0.0.1");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Network", meta = (AllowPrivateAccess = "true"))
	FString Port = TEXT("30000");
};

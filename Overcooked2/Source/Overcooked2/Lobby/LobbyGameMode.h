// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

UENUM(BlueprintType)
enum class ELobbyGameModeState : uint8
{
	ELS_None	UMETA(DisplayName = "None"),
	ELS_Stay    UMETA(DisplayName = "Stay"),
};

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ALobbyGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void PostLogin(APlayerController* NewPlayerController) override;
	
public:
	void EntryStay();
	void Stay(float DeltaTime);

private:
	ELobbyGameModeState CurState = ELobbyGameModeState::ELS_None;
	TArray<APlayerController*> PlayerControllers;

};

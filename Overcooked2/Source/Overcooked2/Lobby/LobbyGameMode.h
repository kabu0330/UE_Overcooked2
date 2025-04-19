// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Global/GameMode/OC2GameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ALobbyGameMode : public AOC2GameMode
{
	GENERATED_BODY()

public:
	ALobbyGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void InitGameState() override;
	virtual void PostLogin(APlayerController* NewPlayerController) override;
	
public:
	int GetUserCount()
	{
		return CurUserCount;
	}

private:
	TArray<APlayerController*> PlayerControllers;
	TArray<FString> ChefHeadNames;
	int CurIdx = 0;
	int CurUserCount = 0;
};

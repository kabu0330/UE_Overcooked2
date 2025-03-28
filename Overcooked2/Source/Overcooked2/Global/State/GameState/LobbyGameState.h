// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Global/State/OC2GameState.h"
#include "LobbyGameState.generated.h"

class ALobbyManager;

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ALobbyGameState : public AOC2GameState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	ALobbyManager* GetLobbyManager() const;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadWrite)
	ALobbyManager* LobbyManager = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ALobbyManager> LobbyManagerClass = nullptr;

};

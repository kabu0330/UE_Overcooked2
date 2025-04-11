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
	ALobbyGameState();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
public:
	UFUNCTION(BlueprintCallable)
	ALobbyManager* GetLobbyManager() const;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayZoomInAnmationUI();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateUserPanelUI(int UserIndex);

	void UpdateChefTexture();

public:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadWrite)
	ALobbyManager* LobbyManager = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ALobbyManager> LobbyManagerClass = nullptr;

	const FString LOBBY_LEVEL = "LobbyDevLevel";
	const FString PLAY_LEVEL = "Sushi1-3StageLevel";

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Global/State/OC2GameState.h"
#include "LobbyGameState.generated.h"

class ALobbyManager;
class AOC2Character;

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

	UFUNCTION(Server, Reliable)
	void Server_SpawnClientCharacter(FVector SpawnLocation, const FString& ChefHeadName);

	UFUNCTION(Server, Reliable)
	void Server_NotifyLoadingComplete();

public:
	void InitCharacter();
	void UpdateChefTexture(int Index);
	void CheckClinetLoadingComplete();
	void SetUserCount(int IntUserCount);

public:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadWrite)
	ALobbyManager* LobbyManager = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ALobbyManager> LobbyManagerClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|TitleActor")
	TSubclassOf<AOC2Character> OC2CharacterClass;

	const FString LOBBY_LEVEL = "LobbyDevLevel";
	const FString PLAY_LEVEL = "Sushi1-3StageLevel";

	TArray<int> CompleteArray;

	bool bChecked = false;

};

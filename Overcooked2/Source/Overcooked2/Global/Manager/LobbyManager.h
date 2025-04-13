// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"

#include "Global/OC2Const.h"

#include "LobbyManager.generated.h"

class AOC2Character;

/**
 * 
 */
UCLASS(Blueprintable)
class OVERCOOKED2_API ALobbyManager : public AInfo
{
	GENERATED_BODY()

public:
	ALobbyManager();

	UFUNCTION(BlueprintCallable)
	static ALobbyManager* Get(UWorld* WorldContext);

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|TitleActor")
	TSubclassOf<AOC2Character> OC2CharacterClass;
	
};

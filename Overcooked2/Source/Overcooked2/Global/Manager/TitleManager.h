// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"

#include "Global/OC2Const.h"

#include "TitleManager.generated.h"

class AOC2Character;

/**
 * 
 */
UCLASS(Blueprintable)
class OVERCOOKED2_API ATitleManager : public AInfo
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Global|TitleActor")
	void InitCharacter();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|TitleActor")
	TSubclassOf<AOC2Character> OC2CharacterClass;
};

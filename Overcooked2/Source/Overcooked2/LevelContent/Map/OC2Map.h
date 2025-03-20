// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Global/GameFramework/OC2Actor.h"
#include "OC2Map.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API AOC2Map : public AOC2Actor
{
	GENERATED_BODY()
	
public:
	AOC2Map();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMeshComponent = nullptr;
};

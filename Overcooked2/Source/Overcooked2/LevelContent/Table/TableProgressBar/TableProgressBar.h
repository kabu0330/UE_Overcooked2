// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Global/GameFramework/OC2Actor.h"
#include "TableProgressBar.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ATableProgressBar : public AOC2Actor
{
	GENERATED_BODY()

public:
	ATableProgressBar();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void SetComponentMesh();

	void UpdateLength();


	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurProgress")
	//USceneComponent* ComponentForProgress = nullptr;

	UStaticMeshComponent* BackGroundMeshComponent = nullptr;
	UStaticMeshComponent* ProgressMeshComponent = nullptr;

	bool bDestroy = false;
	
};

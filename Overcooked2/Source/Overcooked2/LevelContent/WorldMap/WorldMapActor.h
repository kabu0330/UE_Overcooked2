// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "WorldMapActor.generated.h"

UCLASS()
class OVERCOOKED2_API AWorldMapActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AWorldMapActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	void PlaySound();

	UFUNCTION(BlueprintCallable)
	void StopSound();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* AudioComponent = nullptr;
};

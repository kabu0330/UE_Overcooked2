// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Global/GameFramework/OC2Actor.h"
#include "Components/AudioComponent.h"

#include "OC2Map.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API AOC2Map : public AActor
{
	GENERATED_BODY()
	
public:
	AOC2Map();

	UFUNCTION(BlueprintCallable)
	void PlaySound();

	UFUNCTION(BlueprintCallable)
	void StopSound();

	UFUNCTION()
	void ReplaySound(UAudioComponent* AudioComponent);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* AudioComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* AmbienceAudioComponent = nullptr;

};

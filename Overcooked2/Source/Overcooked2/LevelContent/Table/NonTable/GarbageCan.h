// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelContent/Table/CookingTable.h"
#include "GarbageCan.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API AGarbageCan : public ACookingTable
{
	GENERATED_BODY()

public:
	AGarbageCan();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual ACooking* Interact(AActor* ChefActor) override 
	{
		return nullptr; 
	};

	UFUNCTION(BlueprintCallable)
	virtual void PlaceItem(ACooking* Item) override;

	void GarbageTransform(float DeltaTime);

	void DestroyIngredient();

	UFUNCTION(NetMulticast, Reliable)
	void PlaySoundEffect();
	void PlaySoundEffect_Implementation();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeSetting", meta = (AllowPrivateAccess = "true"))
	float Timer = 0.0f;
	bool bDestroyGarbage = false;

	//UPROPERTY(Replicated)
	USoundBase* SoundEffect = nullptr;
};

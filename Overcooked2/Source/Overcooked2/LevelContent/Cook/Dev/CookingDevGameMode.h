// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include <Global/GameMode/OC2GameMode.h>
#include <Global/OC2Enum.h>
#include <LevelContent/Cook/Dev/CookingDevPlayerState.h>
#include "CookingDevGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ACookingDevGameMode : public AOC2GameMode
{
	GENERATED_BODY()
	
public:
	ACookingDevGameMode();

	UFUNCTION(BlueprintCallable)
	void SpawnIngredient(EIngredientType Type);


	UFUNCTION(BlueprintCallable)
	class UCookingDevUserWidget* GetWidget()
	{
		return Widget;
	}

	void SetWidget(class UCookingDevUserWidget* UserWidget)
	{
		Widget = UserWidget;
	}

	void AddPlayerState(ACookingDevPlayerState* State)
	{
		for (int i = 0; i < PlayerState.Num(); i++)
		{
			if (State->GetName() == PlayerState[i]->GetName())
			{
				return;
			}
		}
		PlayerState.Add(State);
		int a = 0;
	}

	//UFUNCTION(Reliable, NetMulticast)
	void ChangeState(EIngredientState State);
	//void ChangeState_Implementation(EIngredientState State);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;	

private:
	class UCookingDevUserWidget* Widget = nullptr;
	TArray<ACookingDevPlayerState*> PlayerState;
	
};

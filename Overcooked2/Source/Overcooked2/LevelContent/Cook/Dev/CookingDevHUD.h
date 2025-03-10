// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include <Global/OC2Enum.h>
#include "CookingDevHUD.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ACookingDevHUD : public AHUD
{
	GENERATED_BODY()

public:
	ACookingDevHUD();
	~ACookingDevHUD();

	class UCookingDevUserWidget* GetWidget()
	{
		return Widget;
	}

	void ServerSpawnIngredient(EIngredientType Type);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float _DeltaTime) override;

	void HandleUIInput();


	UPROPERTY(EditAnywhere, Category = "Cooking")
	TSubclassOf<class UCookingDevUserWidget> WidgetSubclass = nullptr;

private:
	class UCookingDevUserWidget* Widget = nullptr;

	class ASpawnManager* SpawnManager = nullptr;

};

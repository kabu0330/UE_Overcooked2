// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelContent/Table/CookingTable.h"
#include "ChoppingTable.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API AChoppingTable : public ACookingTable
{
	GENERATED_BODY()

public:
	AChoppingTable();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual ACooking* Interact(AActor* ChefActor) override;
	
	UFUNCTION(BlueprintCallable)
	void ChopIngredient(AActor* ChefActor);

	void ChoppingIsDone();

	bool IsChoppingDone()
	{
		return bChoppingDone;
	}

	UFUNCTION(BlueprintCallable)
	void TimerUpdate(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	float GetCurTime()
	{
		return CurTime;
	}

	bool IsTimerActivated()
	{
		return bTimerActivated;
	}

	void TimerSwitch(bool bActivated)
	{
		bTimerActivated = bActivated;
	}

	float GetTime()
	{
		return CurTime;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking")
	USceneComponent* ComponentForProgressBar = nullptr;

protected:

private:
	float Timer = 0.0f;
	float CurTime = 0.0f;
	bool bTimerActivated = false;
	bool bChoppingDone = false;

	float Ratio = 0.0f;

	class AOC2Character* ChefPtr = nullptr;

	UPROPERTY()
	class UWidgetComponent* ProgressBarComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Table", meta = (AllowprivateAccess = "true"))
	TSubclassOf<UUserWidget> TSubClassWidget = nullptr;

	class UGaugeTextureWidget* WidgetPtr = nullptr;
	//class ATableProgressBar* ProgressBar = nullptr;
};

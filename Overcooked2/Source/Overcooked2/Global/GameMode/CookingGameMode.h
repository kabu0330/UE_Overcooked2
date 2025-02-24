// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CookingGameMode.generated.h"

class UOrderManageComponent;

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ACookingGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ACookingGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Component", meta = (AllowPrivateAccess = "true"))
	UOrderManageComponent* OrderManager = nullptr;
};

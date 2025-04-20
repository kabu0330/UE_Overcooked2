// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "CookingPlayerController.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnChefInitialized);

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ACookingPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACookingPlayerController();
	~ACookingPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

public:
	UFUNCTION(Server, Reliable)
	void Server_NotifyCookingWidgetReady();

public:
	FOnChefInitialized OnChefInitialized;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|PlayerController", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* MappingContext = nullptr;


};

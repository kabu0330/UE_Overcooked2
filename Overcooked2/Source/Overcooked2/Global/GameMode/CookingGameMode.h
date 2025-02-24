// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CookingGameMode.generated.h"

class UOrderManageComponent;

UENUM(BlueprintType)
enum class ECookingGameModeState : uint8
{
	ECS_None	UMETA(DisplayName = "None"),
	ECS_Stay    UMETA(DisplayName = "Stay"),
	ECS_Stage   UMETA(DisplayName = "Stage"),
	ECS_Score   UMETA(DisplayName = "Score"),
};
/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ACookingGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void PostLogin(APlayerController* NewPlayerController) override;
	
public:
	ACookingGameMode();

	void EntryStay();
	void Stay(float DeltaTime);

	void EntryStage();
	void Stage(float DeltaTime);

	void EntryScore();
	void Score(float DeltaTime);

	void ChangeState(ECookingGameModeState State);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Component", meta = (AllowPrivateAccess = "true"))
	UOrderManageComponent* OrderManager = nullptr;

	TArray<APlayerController*> PlayerControllers;
	
	ECookingGameModeState CurState = ECookingGameModeState::ECS_None;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LevelContent/WorldMap/WorldMapData.h"
#include "WorldPlayer.generated.h"

UCLASS()
class OVERCOOKED2_API AWorldPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AWorldPlayer();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnMove(const FVector2D& _Vec);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Reliable, NetMulticast)
	void UpdateBusLocation(FVector _Loc);
	void UpdateBusLocation_Implementation(FVector _Loc);

	UFUNCTION(Reliable, Client)
	void Show_Client();
	void Show_Client_Implementation();

	UFUNCTION(Reliable, Client)
	void Hide_Client();
	void Hide_Client_Implementation();

	void Show();
	void Hide();

	void ChangeState(EStageState _State);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

protected:
	virtual void BeginPlay() override;

private:
	TObjectPtr<USceneComponent> MeshWrapper = nullptr;

	float DefaultGravity = 0.f;

	// Temp
	const FVector START_LOC = FVector(-100.f, 150.f, 100.f);

	UPROPERTY(Replicated)
	FVector WorldPlayerLocation = START_LOC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StageState", meta = (AllowPrivateAccess = "true"))
	EStageState CurStageState = EStageState::None;
};

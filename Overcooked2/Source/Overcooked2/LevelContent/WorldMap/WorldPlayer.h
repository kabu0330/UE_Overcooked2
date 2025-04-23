// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LevelContent/WorldMap/WorldMapData.h"
#include "WorldPlayer.generated.h"

class AWorldGameState;
class UAudioComponent;

UCLASS()
class OVERCOOKED2_API AWorldPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AWorldPlayer();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void OnMove(const FVector2D& _Vec);

	UFUNCTION(BlueprintCallable)
	void OnSelectMap();

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

	void ToggleController(bool _IsOn);
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

protected:
	virtual void BeginPlay() override;

private:
	TObjectPtr<USceneComponent> MeshWrapper = nullptr;

	float DefaultGravity = 0.f;

	static FVector WorldPlayerLocation;

	AWorldGameState* WorldGameState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* AudioComponent = nullptr;
};

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

	UFUNCTION(Reliable, Server)
	void Show();
	void Show_Implementation();

	UFUNCTION(Reliable, Server)
	void Hide();
	void Hide_Implementation();

	void ChangeState(EStageState _State);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

protected:
	virtual void BeginPlay() override;

private:
	void InitParentSceneComp();

	TObjectPtr<USceneComponent> MeshWrapper = nullptr;

	float DefaultGravity = 0.f;

	//UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, Category = "StageState", meta = (AllowPrivateAccess = "true"))
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StageState", meta = (AllowPrivateAccess = "true"))
	EStageState CurStageState = EStageState::None;
};

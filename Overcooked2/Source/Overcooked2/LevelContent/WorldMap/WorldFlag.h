// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "WorldFlag.generated.h"

UENUM()
enum class EStageExplain
{
	Idle = 0,
	ShowExplain
};

UCLASS()
class OVERCOOKED2_API AWorldFlag : public AActor
{
	GENERATED_BODY()
	
public:	
	AWorldFlag();

	UFUNCTION(BlueprintCallable)
	void OnOverlapStarted(AActor* _OtherActor);

	UFUNCTION(BlueprintCallable)
	void OnOverlapFinished(AActor* _OtherActor);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	void ChangeState(EStageExplain _State);

	void OnIdle();
	void OnShowExplain();

	EStageExplain State = EStageExplain::Idle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stage", meta = (AllowPrivateAccess = "true"))
	int StageCount = 1;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* FlagMeshComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* FloorMeshComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxCollision = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Dialogue", meta = (AllowPrivateAccess = "true"))
	class AWorldMapDialogue* WorldMapDialogue = nullptr;
};

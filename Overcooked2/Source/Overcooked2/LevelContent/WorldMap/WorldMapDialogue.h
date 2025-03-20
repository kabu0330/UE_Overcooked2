// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldMapDialogue.generated.h"

UCLASS()
class OVERCOOKED2_API AWorldMapDialogue : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldMapDialogue();

	virtual void Tick(float DeltaTime) override;

	void SetStage(int _Stage);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void InitCameraToLook();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DialogMeshComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* CaptureMeshComponent = nullptr;
};

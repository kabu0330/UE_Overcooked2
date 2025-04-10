// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CityCar.generated.h"

UCLASS()
class OVERCOOKED2_API ACityCar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACityCar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMeshComponent = nullptr;

	float UniqueShakeOffset = 0.0f;

};

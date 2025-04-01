// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterCaptureActor.generated.h"

UCLASS()
class OVERCOOKED2_API ACharacterCaptureActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterCaptureActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OC2UI", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DefaultScene = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OC2UI", meta = (AllowPrivateAccess = "true"))
	class USceneCaptureComponent2D* SceneCaptureComponent2D = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OC2UI", meta = (AllowPrivateAccess = "true"))
	class AOC2Character* TargetCharacter;

};

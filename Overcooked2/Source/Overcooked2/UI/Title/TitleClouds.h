// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSprite.h"

#include "TitleClouds.generated.h"

UCLASS()
class OVERCOOKED2_API ATitleClouds : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATitleClouds();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clouds", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clouds", meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* PaperSpriteComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clouds", meta = (AllowPrivateAccess = "true"))
	TArray<UPaperSprite*> PaperSprites;


};

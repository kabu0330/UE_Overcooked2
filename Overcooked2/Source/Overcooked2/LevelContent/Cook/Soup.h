// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Soup.generated.h"

UCLASS()
class OVERCOOKED2_API ASoup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASoup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	USkeletalMeshComponent* SoupSkeletalMeshComponent = nullptr;
};

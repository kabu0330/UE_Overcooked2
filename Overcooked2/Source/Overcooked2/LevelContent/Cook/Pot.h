// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelContent/Cook/Cooking.h"
#include "Pot.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API APot : public ACooking
{
	GENERATED_BODY()
	
public:
	APot();

	void Boiling(ACooking* Rice);

	UFUNCTION(BlueprintCallable)
	void CookRice();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	USkeletalMeshComponent* SoupSkeletalMeshComponent = nullptr;

};

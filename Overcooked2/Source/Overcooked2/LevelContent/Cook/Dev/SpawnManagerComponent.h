// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Global/OC2Enum.h>
#include "SpawnManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVERCOOKED2_API USpawnManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnManagerComponent();

	UFUNCTION(BlueprintCallable)
	void SetIngredientType(EIngredientType Type);

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void RequestSpawnIngredient(EIngredientType Type);
	void RequestSpawnIngredient_Implementation(EIngredientType Type);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private:	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	EIngredientType IngredientType = EIngredientType::EIT_NONE;	
};

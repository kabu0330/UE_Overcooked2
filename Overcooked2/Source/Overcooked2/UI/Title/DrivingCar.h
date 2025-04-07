// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "DrivingCar.generated.h"

UCLASS()
class OVERCOOKED2_API ADrivingCar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrivingCar();

	UFUNCTION(BlueprintCallable)
	void SetLoopTime(float Time)
	{
		LoopCooldown = Time;
	}

	UFUNCTION(BlueprintCallable)
	void SetNiagaraOffset(FTransform Trans);

	void CreateDynamicMaterial();

	UFUNCTION(BlueprintCallable)
	void ChangeColor(FVector4 Color);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:	
	float TimeElapsed = 0.0f;
	FVector InitPos = FVector::ZeroVector;
	float LoopCooldown = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMeshComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "FX", meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent* NiagaraComponent = nullptr;

	UMaterialInstanceDynamic* MaterialInstanceDynamic = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* Niagara = nullptr;

};

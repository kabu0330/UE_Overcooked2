// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include <Global/Component/TimeEventComponent.h>
#include "NiagaraControllActor.generated.h"

UCLASS()
class OVERCOOKED2_API ANiagaraControllActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANiagaraControllActor();

	UFUNCTION(BlueprintCallable)
	void LoopCooldown(float Cooldown, float Duration = 3.0f, float InitDelay = 0.0f)
	{
		bUseCycle = true;
		CycleCooldown = Cooldown;
		RenderDuration = Duration;
		InitDelayTime = InitDelay;
	}

	UFUNCTION(BlueprintCallable)
	void InitDelay(float InitDelay = 0.0f)
	{
		InitDelayTime = InitDelay;
	}

	UFUNCTION(BlueprintCallable)
	void SetRelativeOffset(FTransform Transform)
	{
		SetActorRelativeLocation(Transform.GetLocation());
		SetActorRelativeRotation(Transform.GetRotation());
		SetActorRelativeScale3D(Transform.GetScale3D());
	}

	UFUNCTION(BlueprintCallable)
	void SetActivate(bool Value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateLoopCycle(float DeltaTime);

private:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserEdit", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* NiagaraComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserEdit", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* NiagaraSystem = nullptr;

	UTimeEventComponent* TimeEventComponent = nullptr;

	float CycleCooldown = 0.0f;

	float TimeElpased = 0.0f;

	float RenderDuration = 0.0f;
	float InitDelayTime = 0.0f;

	bool bUseCycle = false;
	bool bCanCalled = false;
	bool bLoopStart = false;
	bool bIsFirstCalled = false;
};

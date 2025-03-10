// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Global/Interface/Interactable.h"
#include "OC2Actor.generated.h"

UCLASS()
class OVERCOOKED2_API AOC2Actor : public AActor//, public IInteractable
{
	GENERATED_BODY()

public:
	AOC2Actor();

public:
	virtual void InitOC2Actor();
	
public:

	// 머티리얼 하이라이트 효과 적용
	UFUNCTION(BlueprintCallable)
	virtual void ApplyMaterialHighlight();

	// 머티리얼 하이라이트 효과를 제거
	UFUNCTION(BlueprintCallable)
	virtual void RestoreMaterial();

	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetStaticMeshComponent()
	{
		return StaticMeshComponent;
	}

	UFUNCTION(BlueprintCallable)
	bool IsHighlighted()
	{
		return bIsHighlighted;
	}

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	TArray<UMaterialInterface*> Materials;
	float DiffuseColorMapWeight = 0.0f;
	bool bIsHighlighted = false;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SecenComponent = nullptr;
public:

};

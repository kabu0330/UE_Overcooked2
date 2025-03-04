// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Global/Interface/Interactable.h"
#include "OC2Actor.generated.h"

USTRUCT(BlueprintType)
struct FMaterialData
{
	GENERATED_BODY()
public:
	UMaterialInterface* Material = nullptr;
	float DiffuseColorMapWeight = 0.0f;
};

UCLASS()
class OVERCOOKED2_API AOC2Actor : public AActor//, public IInteractable
{
	GENERATED_BODY()
	
public:	
	AOC2Actor();

	// ��Ƽ���� ���̶���Ʈ ȿ�� ����
	UFUNCTION(BlueprintCallable)
	virtual void ApplyMaterialHighlight();

	void ApplyMaterialHighlight(UMeshComponent* Mesh);

	// ��Ƽ���� ���̶���Ʈ ȿ���� ����
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

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//TArray<UMaterialInterface*> Materials;
	//float DiffuseColorMapWeight = 0.0f;
	bool bIsHighlighted = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	TMap<UMeshComponent*, TArray<FMaterialData>> Materials;
public:	

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "NiagaraSystem.h"

#include "ResourceNiagaraTable.generated.h"

USTRUCT(BlueprintType)
struct FResourceNiagaraDataRow : public FTableRowBase
{
	GENERATED_BODY()

	FResourceNiagaraDataRow() {}
	~FResourceNiagaraDataRow() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	UNiagaraSystem* NiagaraSystem = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	FRotator Rotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	FVector Scale = FVector::ZeroVector;
};

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UResourceNiagaraTable : public UObject
{
	GENERATED_BODY()
	
};

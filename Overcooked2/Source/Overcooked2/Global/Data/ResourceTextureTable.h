// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ResourceTextureTable.generated.h"

// 일반 재료 데이터 열
USTRUCT(BlueprintType)
struct FResourceTextureDataRow : public FTableRowBase
{
	GENERATED_BODY()

	FResourceTextureDataRow() {}
	~FResourceTextureDataRow() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global|Data")
	UTexture2D* TextureRes = nullptr;
};


/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UResourceTextureTable : public UObject
{
	GENERATED_BODY()
	
};

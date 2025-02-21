// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OC2GameInstance.generated.h"

class UDataTable;

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UOC2GameInstance : public UGameInstance
{
	GENERATED_BODY()

	//friend class UGlobla

public:
	UOC2GameInstance();
	~UOC2GameInstance();

private:
	UPROPERTY(VisibleAnywhere, Category = "Global|Data")
	UDataTable* DataTables = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Global|Data")
	UDataTable* ActorDataTable = nullptr;
	
};

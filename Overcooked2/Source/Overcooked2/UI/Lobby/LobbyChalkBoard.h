// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "LobbyChalkBoard.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ALobbyChalkBoard : public AStaticMeshActor
{
	GENERATED_BODY()
public:
	ALobbyChalkBoard(const FObjectInitializer& ObjectInitializer);

	class UWidgetComponent* GetWidgetComponent()
	{
		return WidgetComponent;
	}

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWidgetComponent* WidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<class ULobbyUserWidget> ChalkBoardWidgetClass = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWidgetInteractionComponent* WidgetInteraction;

};

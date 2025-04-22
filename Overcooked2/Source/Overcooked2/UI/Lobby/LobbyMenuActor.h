// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LobbyMenuActor.generated.h"

UCLASS()
class OVERCOOKED2_API ALobbyMenuActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALobbyMenuActor();

	class UWidgetComponent* GetWidgetComponent()
	{
		return WidgetComponent;
	}

	UFUNCTION(BlueprintCallable, Category = "OC2UI")
	void RemoveHoverEvent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWidgetInteractionComponent* WidgetInteraction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWidgetComponent* WidgetComponent;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<class ULobbyMenuWidget> LobbyMenuWidgetClass = nullptr;

private:
	class ULobbyMenuWidget* LobbyMenuWidget = nullptr;

};

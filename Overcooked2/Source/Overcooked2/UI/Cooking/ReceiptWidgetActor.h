// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReceiptWidgetActor.generated.h"

UCLASS()
class OVERCOOKED2_API AReceiptWidgetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReceiptWidgetActor();
	void ShowTotalScoreAnim();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWidgetComponent* WidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<class UCookingReceiptWidget> ReceiptWidgetClass = nullptr;

private:
	class UCookingReceiptWidget* CookingReceiptWidget = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

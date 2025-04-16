// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Global/Component/TimeEventComponent.h>
#include <UI/Loading/LoadingWidget.h>
#include "TimeEventActor.generated.h"

UCLASS()
class OVERCOOKED2_API ATimeEventActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimeEventActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:	
	ULoadingWidget* LoadingWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> SubclassWidget = nullptr;

	UTimeEventComponent* TimeEventComponent = nullptr;

};

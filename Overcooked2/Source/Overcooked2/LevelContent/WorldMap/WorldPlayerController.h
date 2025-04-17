// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WorldPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API AWorldPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AWorldPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:

	UFUNCTION(BlueprintCallable)
	void AddInputMappingContext(UInputMappingContext* _Context);

	UFUNCTION(Server, Reliable)
	void Server_SetViewTarget(AActor* NewCamera);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(Server, Reliable)
	void Server_NotifyLoadingComplete();

private:
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category="Controller", meta=(AllowPrivateAccess=true))
	UInputMappingContext* MappingContext = nullptr;

private:
	bool bReported = false;
};

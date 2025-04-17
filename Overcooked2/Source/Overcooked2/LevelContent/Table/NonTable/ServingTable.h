// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelContent/Table/CookingTable.h"
#include "ServingTable.generated.h"

class APlateSpawner;

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API AServingTable : public ACookingTable
{
	GENERATED_BODY()

public:
	AServingTable();


	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void PlaceItem(ACooking* ReceivedCooking) override;

	UFUNCTION(Server, Reliable)
	void Server_SpawnPlateInPlateSpawner();

	ACooking* GetCookingPtr()
	{
		return CookingPtr;
	}

	void CookingRight()
	{
		bCookingRight = true;
	}

	void CookingWrong()
	{
		bCookingWrong = true;
	}

	UFUNCTION(NetMulticast, Reliable)
	void PlaySoundEffect();
	void PlaySoundEffect_Implementation();

	void InitNiagara();
	void SetNiagara(class UNiagaraComponent* NiagaraComponent, bool IsActivate);
	void ShowServingEffet();

	UFUNCTION(Server, Reliable)
	void ShowEffect();
	void ShowEffect_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void NetMuticast_ShowEffect();
	void NetMuticast_ShowEffect_Implementation();

	UFUNCTION()
	void HandleNiagaraFinished(UNiagaraComponent* NiagaraComponent);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_CanEffect();
	void NetMulticast_CanEffect_Implementation();

private:
	//void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CookingCheck", meta = (AllowPrivateAccess = "true"))
	bool bCookingRight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CookingCheck", meta = (AllowPrivateAccess = "true"))
	bool bCookingWrong = false;

	UPROPERTY()
	APlateSpawner* PlateSpawner = nullptr;

	USoundBase* SoundEffect = nullptr;


	class UNiagaraComponent* RingNiagaraComponent = nullptr;
	class UNiagaraComponent* FlashNiagaraComponent = nullptr;
	class UNiagaraComponent* ConfettiNiagaraComponent = nullptr;

	class UTimeEventComponent* TimeEventComponent = nullptr;

	UPROPERTY()
	bool bCanEffet = false;
};


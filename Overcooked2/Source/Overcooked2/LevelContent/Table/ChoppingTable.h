// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelContent/Table/CookingTable.h"
#include "ChoppingTable.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API AChoppingTable : public ACookingTable
{
	GENERATED_BODY()

public:
	AChoppingTable();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(BlueprintCallable)
	virtual ACooking* Interact(AActor* ChefActor) override;
	
	UFUNCTION(BlueprintCallable)
	void ChopIngredient(AActor* ChefActor);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void HideProgressBar(bool Value);
	void HideProgressBar_Implementation(bool Value);

	//UFUNCTION(NetMulticast, Reliable)
	UFUNCTION(Server, Reliable)
	void ChoppingIsDone();
	void ChoppingIsDone_Implementation();

	void InitProgressBar();

	bool IsChoppingDone()
	{
		return bChoppingDone;
	}

	UFUNCTION(BlueprintCallable)
	void TimerUpdate(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	float GetCurTime()
	{
		return CurTime;
	}

	bool IsTimerActivated()
	{
		return bTimerActivated;
	}

	void TimerSwitch(bool bActivated)
	{
		bTimerActivated = bActivated;
	}

	float GetTime()
	{
		return CurTime;
	}

	//bool CheckChoppingIngredeint()
	//{
	//	return bChopping;
	//}

	void CheckChefIsChopping();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking")
	//USceneComponent* ComponentForProgressBar = nullptr;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Cooking")
	class UStaticMeshComponent* KnifeMeshComponent = nullptr;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Cooking")
	bool bCheckHidden = false;

	//UFUNCTION(BlueprintCallable)
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void HideKnife();
	void HideKnife_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void PlaySoundEffect();
	void PlaySoundEffect_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void StopSoundEffect();
	void StopSoundEffect_Implementation();

	void InitNiagara();

	UFUNCTION()
	void OnRep_SetNiagaraActive();

	UFUNCTION()
	void SetNiagaraActive();


protected:

private:
	UPROPERTY(Replicated)
	float Timer = 0.0f;

	float CurTime = 0.0f;

	UPROPERTY(Replicated)
	bool bTimerActivated = false;

	UPROPERTY(Replicated)
	bool bChopping = false;

	UPROPERTY(Replicated)
	bool bChoppingDone = false;

	UPROPERTY(ReplicatedUsing = OnRep_SetNiagaraActive)
	bool bNiagaraActive = false;

	UPROPERTY(Replicated)
	float Ratio = 0.0f;

	UPROPERTY(Replicated)
	class AOC2Character* ChefPtr = nullptr;

	UPROPERTY(Replicated)
	class UWidgetComponent* ProgressBarComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Table", meta = (AllowprivateAccess = "true"))
	TSubclassOf<UUserWidget> TSubClassWidget = nullptr;

	class UGaugeTextureWidget* WidgetPtr = nullptr;
	//class ATableProgressBar* ProgressBar = nullptr;

	UPROPERTY(Replicated)
	class UAudioComponent* AudioComponent = nullptr;

	USoundBase* SoundEffect = nullptr;

	class UNiagaraComponent* NiagaraComponent0 = nullptr;
	class UNiagaraComponent* NiagaraComponent1 = nullptr;
};

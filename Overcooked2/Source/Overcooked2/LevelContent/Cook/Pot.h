// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelContent/Cook/Cooking.h"

#include "Global/OC2Const.h"
#include "Global/OC2Struct.h"
#include "Global/OC2Enum.h"

#include "Pot.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API APot : public ACooking
{
	GENERATED_BODY()
	
public:
	APot();

	// 처음 재료가 들어왔을 때 호출할 함수
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Add(class AIngredient* Ingredient);
	void Add_Implementation(class AIngredient* Ingredient);

	// 조리된 밥을 받아오는 함수
	UFUNCTION(BlueprintCallable)
	class AIngredient* GetRice();

	// 조리가 완료된 상태인지 결과 반환 함수
	bool IsCooked() const
	{
		return bIsCooked;
	}

	EPotState GetPotState() const
	{
		return PotState;
	}

	// 냄비에 들어있는 모든 데이터 삭제
	void ResetPot();
	
	bool IsCombinationSuccessful() const
	{
		return bIsCombinationSuccessful;
	}
	 
	// 냄비에 쌀이 들어있는지 결과를 반환하는 함수
	bool IsRiceInPot() const
	{
		return bIsRiceInPot;
	}

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UMaterialInstanceDynamic* LoadNoneMaterial();
	void Cook(float DeltaTime);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void SetAction();
	void SetAction_Implementation();

	void ChangeMaterialColor(FVector4 Color);

	UFUNCTION()
	void SetSoupMaterial();


	bool IsBoiling();

	void BlinkTexture(float DeltaTime);

	void ChangeNoneMaterial();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	virtual void ForwardCookingTable(class ACookingTable* Table) override;
	virtual void ForwardAttachToChef() override;
	virtual void ForwardDetachToChef() override;

	void InitTexture();

	void SetTexture(const FString& RowName);
	void SetWarnigTextureOffset();


	void ChangeState(EPotState CurState, EPotState NextState, float TransitionTime);
	void ChangeSoupColor(float DeltaTime);


private:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	USkeletalMeshComponent* SoupSkeletalMeshComponent = nullptr; // 쌀이 들어오면 렌더링될 메시

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	TArray<UMaterialInstanceDynamic*> SoupDynamicMaterial; // 조리 시간에 따라 색상 바꿀 머티리얼들

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	UMaterialInstanceDynamic* NoneMaterial = nullptr; // 투명 머티리얼

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	EPotState PotState = EPotState::IDLE; // 현재 상태

	UPROPERTY(/*Replicated,*/ EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	EPotState PrevPotState = EPotState::MAX; // 직전 상태. 현재 상태와 비교해서 상태 변경처리

	UPROPERTY(Replicated)
	bool bIsCombinationSuccessful = false; // 쌀이 냄비에 넣는데 성공했는지

	UPROPERTY(Replicated)
	bool bIsRiceInPot = false; // 쌀이 냄비에 들어있는지

	UPROPERTY(Replicated)
	bool bIsCooked = false; // 조리 완료 상태인지 State가 여러개가 조리 완료 상태라 bool 변수로 대체
	
	UPROPERTY(Replicated)
	float TimeElapsed = 0.0f; // 조리 시간

	UPROPERTY(Replicated)
	class ACookingTable* CookingTable = nullptr; // 버너 테이블 위에 있는지 체크할 포인터 변수

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	class UBillboardComponent* TextureBillboard = nullptr; // 텍스처

	// 경고 텍스처 블링크 효과
	UPROPERTY()
	float BlinkTimeElapsed = 0.0f; 

	UPROPERTY()
	float BlinkTime = 0.0f;

	UPROPERTY()
	bool bCanBlink = false;
	//

	
	// 머티리얼 색상을 조리시간 경과에 따라 변경
	UPROPERTY()
	FLinearColor CurrentColor = FLinearColor::Blue;	// 현재 색상

	UPROPERTY(Replicated)
	FLinearColor TargetColor = FLinearColor::Blue;	// 목표 색상

	UPROPERTY()
	float ColorChangeSpeed = 1.5f;  // 색상이 바뀌는 속도

	UPROPERTY()
	bool bColorChanging = false;    // 색상이 변하는 중인지
	// 


	UPROPERTY()
	class UTimeEventComponent* TimeEventComponent = nullptr;

	FVector InitPos = FVector(249, 1452, 60);

};

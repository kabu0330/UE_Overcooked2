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
	UFUNCTION(NetMulticast, Reliable)
	void ResetPot();
	void ResetPot_Implementation();
	
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
	// override
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Attach & Deatach
	virtual void ForwardCookingTable(class ACookingTable* Table) override;
	virtual void ForwardAttachToChef() override;
	virtual void ForwardDetachToChef() override;

	// Network
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;


	// 냄비 상태 동기화 함수, Pot 로직 핵심 함수
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void SetAction(); 
	void SetAction_Implementation();

	bool IsBoiling();
	void Cook(float DeltaTime);
	void ChangeState(EPotState CurState, EPotState NextState, float TransitionTime);


	// Mesh And Dynamic Material
	UMaterialInstanceDynamic* LoadNoneMaterial();
	void ChangeNoneMaterial();

	UFUNCTION()
	void SetSoupMaterial();
	void ChangeSoupColor(float DeltaTime);
	void ChangeMaterialColor(FVector4 Color);
	void UpdateChangingColor();


	// Texture
	void InitGaugeWidget();
	void InitStatusWidget();
	void InitIconWidget();

	void InitWidgetSetting(class UWidgetComponent* WidgetComponent);
	void UpdateGaugeWidget();

	UTexture2D* GetTexture(const FString& RowName);
	void SetIcon(const FString& RowName);


	// Blink Effect
	void CanBlink();
	void BlinkTexture(float DeltaTime);
	void UpdateTextureVisibilityOnTable();


	// Niagara
	void InitNiagara();
	void SetNiagara(bool IsActivate);
	void SetNiagaraAsset(const FString& Name);

private:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	USkeletalMeshComponent* SoupSkeletalMeshComponent = nullptr; // 쌀이 들어오면 렌더링될 메시

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> SoupDynamicMaterial; // 조리 시간에 따라 색상 바꿀 머티리얼들

	UPROPERTY(Replicated)
	UMaterialInstanceDynamic* NoneMaterial = nullptr; // 투명 머티리얼

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	EPotState PotState = EPotState::IDLE; // 현재 상태

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	EPotState PrevPotState = EPotState::MAX; // 직전 상태. 현재 상태와 비교해서 상태 변경처리

	UPROPERTY(Replicated)
	bool bIsCombinationSuccessful = false; // 쌀이 냄비에 넣는데 성공했는지

	UPROPERTY(Replicated)
	bool bIsRiceInPot = false; // 쌀이 냄비에 들어있는지

	UPROPERTY(Replicated)
	bool bIsCooked = false; // 조리 완료 상태인지 State가 여러개가 조리 완료 상태라 bool 변수로 대체
	
	UPROPERTY(Replicated)
	float TimeElapsed = 0.0f; // 조리 시간


	// 조리 게이지
	UPROPERTY()
	class UWidgetComponent* GaugeWidgetComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	TSubclassOf<UUserWidget> SubclassGaugeWidget = nullptr; // 에디터에서 가져올 WBP 지정

	UPROPERTY()
	class UGaugeTextureWidget* GaugeWidget = nullptr; // 세팅한 위젯 객체 저장 및 함수 호출용

	float CookingTimeRatio = 0.0f; // Widget으로 보내 요리 진행률을 나타낼 변수
	//

	// 조리 완료 및 경고 텍스처 출력 위젯
	UPROPERTY()
	class UWidgetComponent* StatusWidgetComponent = nullptr; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	TSubclassOf<UUserWidget> SubclassStatusWidget = nullptr; // 에디터에서 가져올 WBP 지정

	UPROPERTY()
	class UCookStatusWidget* StatusWidget = nullptr; // 세팅한 위젯 객체 저장 및 함수 호출용
	
	// 경고 텍스처 블링크 효과
	UPROPERTY()
	float BlinkTimeElapsed = 0.0f;

	UPROPERTY()
	float BlinkTime = 0.0f;

	UPROPERTY(Replicated)
	bool bIsBlinking = false; // 상태가 변할 때 블링크 가능 여부를 결정짓는 변수

	UPROPERTY(Replicated)
	bool bCanBlink = false; // 플레이어가 냄비를 들고 내릴 때 블링크 여부를 결정짓는 변수
	//

	// 
	class UWidgetComponent* IconWidgetComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	TSubclassOf<UUserWidget> SubclassIconWidget = nullptr; // 에디터에서 가져올 WBP 지정

	UPROPERTY()
	class UIngredientIconWidget* IconWidget = nullptr; // 세팅한 위젯 객체 저장 및 함수 호출용


	UPROPERTY(Replicated)
	class ACookingTable* CookingTable = nullptr; // 버너 테이블 위에 있는지 체크할 포인터 변수

	
	// 머티리얼 색상을 조리시간 경과에 따라 변경
	UPROPERTY()
	FLinearColor CurrentColor = FLinearColor::Blue;	// 현재 색상

	UPROPERTY(Replicated)
	FLinearColor TargetColor = FLinearColor::Blue;	// 목표 색상

	UPROPERTY()
	float ColorChangeSpeed = 1.5f;  // 색상이 바뀌는 속도

	UPROPERTY()
	bool bColorChanging = false;    // 색상이 변하는 중인지

	UPROPERTY(Replicated)
	bool bCanColorChange = false; // 색상이 변할 수 있는 상태인지, 캐릭터가 들고 내릴 때 판단
	// 


	// 이펙트
	UPROPERTY(Replicated)
	class UNiagaraComponent* NiagaraComponent = nullptr;



	UPROPERTY()
	class UTimeEventComponent* TimeEventComponent = nullptr;

	FVector InitPos = FVector(249, 1452, 60);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <LevelContent/Cook/Cooking.h>
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Ingredient.generated.h"

// 요리 재료
UCLASS()
class OVERCOOKED2_API AIngredient : public ACooking
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AIngredient();

	// Network
	UFUNCTION(BlueprintCallable, Reliable, Server)
	void SetType(EIngredientType Type);
	void SetType_Implementation(EIngredientType Type);



	// 상자에서 꺼냈을 때
	UFUNCTION(BlueprintCallable)
	AIngredient* Init(EIngredientType Type);

	// 조리 완료 시, 캐릭터(Server RPC 함수) -> 테이블 -> 재료순 호출
	UFUNCTION(BlueprintCallable, Reliable, NetMulticast)
	void ChangeState(EIngredientState State);
	void ChangeState_Implementation(EIngredientState State);

	// 도마에서 썰어야 하는 재료야?
	UFUNCTION(BlueprintCallable)
	bool IsChoppable() const
	{
		return CurIngredientState == EIngredientState::EIS_CHOPPABLE;
	}

	// 프라이팬에 구워야 하는 재료야?
	UFUNCTION(BlueprintCallable)
	bool IsGrillable() const
	{
		return CurIngredientState == EIngredientState::EIS_GRILLABLE;
	}

	// 솥에서 밥을 지을 수 있어?
	UFUNCTION(BlueprintCallable)
	bool IsBoilable() const
	{
		return CurIngredientState == EIngredientState::EIS_BOILABLE;
	}

	UFUNCTION(BlueprintCallable)
	EIngredientType GetIngredientType() const
	{
		return IngredientType;
	}

	UFUNCTION(BlueprintCallable)
	EIngredientState GetCurIngredientState() const
	{
		return CurIngredientState;
	}

	UFUNCTION(BlueprintCallable)
	bool IsCooked();


	UFUNCTION(NetMulticast, Reliable)
	void SetThrowing(bool IsThrowing);
	void SetThrowing_Implementation(bool IsThrowing)
	{
		bIsThrowing = IsThrowing;
	}

	bool IsThrowing() const
	{
		return bIsThrowing;
	}


	UFUNCTION(NetMulticast, Reliable)
	void SetThrower(AActor* Actor);
	void SetThrower_Implementation(AActor* Actor)
	{
		Thrower = Actor;
	}

	AActor* GetThrower() const
	{
		return Thrower;
	}


	UFUNCTION(BlueprintCallable)
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);


	//class UBillboardComponent* GetTextureBillboard()
	//{
	//	return TextureBillboard;
	//}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	const FIngredientCookDataRow& CheckState(EIngredientState State);

	void Offset(FVector Pos, FRotator Rot, FVector Scale);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	void SetIngredientData(UOC2GameInstance* Inst, FName Name);
	void SetLocalOffset();
	void SetTexture();

	void CreateDynamicMaterial();

	void InitIconWidget();

private:
	void DeactivateHighlight();
	void ActivateHighlight();


	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	EIngredientType IngredientType = EIngredientType::EIT_NONE;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	EIngredientState CurIngredientState = EIngredientState::EIS_NONE;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	bool bIsThrowing = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	AActor* Thrower = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	class UWidgetComponent* WidgetComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	TSubclassOf<UUserWidget> SubclassWidget = nullptr; // 에디터에서 가져올 WBP 지정

	UPROPERTY()
	class UIngredientIconWidget* IconWidget = nullptr; // 세팅한 위젯 객체 저장 및 함수 호출용
	
};

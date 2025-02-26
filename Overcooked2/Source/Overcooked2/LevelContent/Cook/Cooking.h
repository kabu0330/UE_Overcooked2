// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cooking.generated.h"

// Ingredient(요리 재료)와 Plate(요리)의 최상위 클래스 
UCLASS()
class OVERCOOKED2_API ACooking : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACooking();

	// BP에서 설정한 DishMeshes의 인덱스를 바꿔서 다른 메시를 출력
	UFUNCTION(BlueprintCallable, Category = "Cooking")
	void SetMeshByIndex(int32 Index);

	// 디버깅용 함수, 2초 뒤에 해당 인덱스의 메시로 바꿔라.
	UFUNCTION(BlueprintCallable, Category = "Cooking")
	void SetDebugMeshByIndex(int32 Index);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 메시 지정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<UStaticMesh*> DishMeshes;

	// 현재 눈에 보이는 메시
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* DishMeshComponent;

	class UTimeEventActorComponent* TimeEvent = nullptr;

private:	
	// ECookingType CookingType = ECookingType::NONE;
};

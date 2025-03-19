// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Global/Interface/Interactable.h"
#include "OC2Actor.generated.h"

UCLASS()
class OVERCOOKED2_API AOC2Actor : public AActor//, public IInteractable
{
    GENERATED_BODY()

public:
    /** 기본 생성자 */
    AOC2Actor();

public:
    /** OC2Actor 초기화 함수 */
    virtual void InitOC2Actor();

public:
    /** 머티리얼 하이라이트 효과 적용 */
    UFUNCTION(BlueprintCallable, Reliable, NetMulticast)
    virtual void ApplyMaterialHighlight();

    /** 머티리얼 하이라이트 효과 제거 */
    UFUNCTION(BlueprintCallable, Reliable, NetMulticast)
    virtual void RestoreMaterial();

    /**
     * StaticMeshComponent 반환
     * @return StaticMeshComponent의 포인터
     */
    UFUNCTION(BlueprintCallable)
    UStaticMeshComponent* GetStaticMeshComponent()
    {
        return StaticMeshComponent;
    }

    /**
     * 하이라이트 상태 확인
     * @return 현재 하이라이트 여부 (true: 하이라이트 적용됨, false: 미적용)
     */
    UFUNCTION(BlueprintCallable)
    bool IsHighlighted()
    {
        return bIsHighlighted;
    }

    /** 클라이언트가 서버에게 액터 삭제 요청을 보낼 함수 (RPC) */
    UFUNCTION(BlueprintCallable, Category = "Global")
    void RequestOC2ActorDestroy();

    /** 서버에서 액터를 삭제하는 함수 (RPC 구현) */
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_DestroyOC2Actor();

    /**
     * 네트워크 동기화를 위한 Replicated 프로퍼티 등록
     * @param OutLifetimeProps 네트워크 동기화할 변수 목록
     */
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    /** 액터 시작 시 호출되는 함수 */
    virtual void BeginPlay() override;

    /**
     * 매 프레임마다 호출되는 함수
     * @param DeltaTime 프레임 간 시간 간격
     */
    virtual void Tick(float DeltaTime) override;

    /** 액터의 머티리얼 리스트 */
    TArray<UMaterialInterface*> Materials;

    /** 머티리얼의 DiffuseColorMap 가중치 리스트 */
    TArray<float> DiffuseColorMapWeights;

    /** 하이라이트 적용 여부 */
    bool bIsHighlighted = false;

    /** 액터의 StaticMeshComponent */
    UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* StaticMeshComponent = nullptr;

    /** 액터의 SceneComponent */
    UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
    USceneComponent* SecenComponent = nullptr;
};

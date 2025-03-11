// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "Global/GameFramework/OC2Actor.h"
#include "LevelContent/Cook/Cooking.h"
#include "LevelContent/Table/CookingTable.h"
#include "Global/Component/TimeEventComponent.h"
#include "OC2Character.generated.h"

USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_BODY()

	FCharacterData() {}
	FCharacterData(UMaterialInterface* InMaterial, int32 Index)
	: Material(InMaterial), MaterialIndex(Index){}
	~FCharacterData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	int32 MaterialIndex;
};

UCLASS()
class OVERCOOKED2_API AOC2Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOC2Character();

	UFUNCTION(BlueprintCallable)
	void MoveCharacter(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	bool IsHolding() { return GrabbedObject != nullptr; }

	UFUNCTION(BlueprintCallable)
	bool IsCooking() { return bIsCooking; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Reliable, Server)
	void CheckDash(float DeltaTime);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetCharacterHead(FString Name);

	UFUNCTION(BlueprintCallable)
	void InitMesh();

	UFUNCTION(BlueprintCallable)
	void CheckInteract();

	// 테이블 또는 접시와의 상호작용
	UFUNCTION(BlueprintCallable, Reliable, Server)
	void Interact();

	UFUNCTION(Reliable, Server)
	void Grab(ACooking* Cook);

	UFUNCTION(Reliable, Server)
	void Drop();

	// 캐릭터의 행동(요리하기, 던지기 등)
	UFUNCTION(BlueprintCallable, Reliable, Server)
	void DoSth();

	UFUNCTION(Reliable, Server)
	void Throwing();

	UFUNCTION(Reliable, Server)
	void Cooking();

	UFUNCTION(Reliable, Server)
	void Dash();

	UFUNCTION(BlueprintCallable)
	void StopDash();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private :

	// 이 함수는 캐릭터의 머리를 설정하는 함수읾
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserInput", meta = (AllowPrivateAccess = "true"))
	float Alpha = 0.0f;

	//Actions -------------------------------------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserInput", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserInput", meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserInput", meta = (AllowPrivateAccess = "true"))
	UInputAction* CharacterAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserInput", meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction = nullptr;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserInput", meta = (AllowPrivateAccess = "true"))
	UMaterial* TransparentMat = nullptr;

	// 상호작용 할 범위 구체에 대한 반지름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserInput", meta = (AllowPrivateAccess = "true"))
	float TraceRadius = 100.0f; 

	// 캐릭터 머리 정보
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UserInput", meta = (AllowPrivateAccess = "true"))
	TMap<FString, FCharacterData> CharacterHeadMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	UTimeEventComponent* TimeEvent;


	// 잡은 오브젝트가 위치할 Transform을 가지고 있는 Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	USceneComponent* GrabComponent;

	// 현재 내가 상호작용할 수 있는 객체의 정보 ( 테이블이나 음식이나 소화기나.. 등등)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	AOC2Actor* SelectedOC2Actor = nullptr;

	// 현재 내가 들고 있는 객체의 정보 
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	ACooking* GrabbedObject = nullptr;

	TPair<int, UMaterialInterface*> Knife;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	bool bIsCooking = false;
	/// <summary>
	/// Dash Variables
	/// </summary>
	/// 
	/// Todo : 대쉬 제대로 안되는거 같아서 나중에 고쳐야 함
	/// 
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	bool bIsDashing = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float DashPower = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float DashDuration = 0.5f;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float DashTimer = 0.0f;
};

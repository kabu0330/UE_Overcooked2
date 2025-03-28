// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "Global/GameFramework/OC2Actor.h"
#include "LevelContent/Cook/Cooking.h"
#include "LevelContent/Table/CookingTable.h"
#include "Global/Component/TimeEventComponent.h"
#include "Components/SphereComponent.h"
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
	UMaterialInterface* Material = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	int32 MaterialIndex = 0;
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
	bool IsCooking() { return bIsChopping; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void CheckDash(float DeltaTime);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void SetCharacterName(const FString& Name);

	UFUNCTION()
	void OnRep_ChangeCharacter();

	UFUNCTION(BlueprintCallable)
	void InitMesh();

	void ClearMaterials();

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
	void DoActionPress();

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void DoActionRelease();

	UFUNCTION(Reliable, Server)
	void Throwing();

	UFUNCTION(Reliable, Server)
	void Chopping(bool State);

	UFUNCTION()
	void OnRep_KnifeSet();


	UFUNCTION(Reliable, Server)
	void Dash();

	UFUNCTION(BlueprintCallable)
	void StopDash();

	UFUNCTION(Reliable, Server)
	void OnOverlapCheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

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
	TArray<FString> IndexToName;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	UTimeEventComponent* TimeEvent;


	// 잡은 오브젝트가 위치할 Transform을 가지고 있는 Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	USceneComponent* GrabComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Plane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CheckOverlap;

	// 현재 내가 상호작용할 수 있는 객체의 정보 ( 테이블이나 음식이나 소화기나.. 등등)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	AOC2Actor* SelectedOC2Actor = nullptr;

	// 현재 내가 들고 있는 객체의 정보 
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	ACooking* GrabbedObject = nullptr;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	bool bCanThrowing = false;

	
	TPair<int, UMaterialInterface*> Knife;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_ChangeCharacter, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	FString CharacterName;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_KnifeSet, BlueprintReadOnly, Category = "Cook", meta = (AllowPrivateAccess = "true"))
	bool bIsChopping = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserInput", meta = (AllowPrivateAccess = "true"))
	float CharacterSpeed = 10.0f;
	/// Dash Variables
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	bool bIsDashing = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float DashPower = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float DashDuration = 0.5f;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float DashTimer = 0.0f;
};

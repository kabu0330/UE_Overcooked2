// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/WorldMap/WorldPlayer.h"
#include "Global/OC2GameInstance.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AWorldPlayer::AWorldPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultGravity = GetCharacterMovement()->GravityScale;
}

void AWorldPlayer::BeginPlay()
{
	Super::BeginPlay();

	InitParentSceneComp();
	Hide();

	SetActorLocation(FVector(-100.f, 150.f, 65.f));

	ChangeState(EStageState::ShowStage1_1);
	//ChangeState_Implementation(EStageState::ShowStage1_1);
}

void AWorldPlayer::Tick(float DeltaTime)
{
	if (Controller)
	{
		AActor* CurrentViewTarget = Controller->GetViewTarget();
		UE_LOG(LogTemp, Warning, TEXT("1. Current View Target: %s"), *CurrentViewTarget->GetName());
	}

	Super::Tick(DeltaTime);

	if (Controller)
	{
		AActor* CurrentViewTarget = Controller->GetViewTarget();
		UE_LOG(LogTemp, Warning, TEXT("2. Current View Target: %s"), *CurrentViewTarget->GetName());
	}
}

void AWorldPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AWorldPlayer::OnMove(const FVector2D& _Vec)
{
	AddMovementInput(FVector(_Vec.X * 0.5f, _Vec.Y * 0.5f, 0.f));
}

void AWorldPlayer::InitParentSceneComp()
{
	TArray<USceneComponent*> Components;
	GetComponents<USceneComponent>(Components);

	for (USceneComponent* Comp : Components)
	{
		// Temp literal
		if (Comp->GetName().Equals("MeshWrapper"))
		{
			MeshWrapper = Comp;
		}
	}
}

void AWorldPlayer::Show()
{
	// Temp
	UCapsuleComponent* CapComponent = GetCapsuleComponent();
	CapComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	/*CapComponent->SetVisibility(true);
	CapComponent->SetComponentTickEnabled(true);
	CapComponent->SetActive(true);*/

	if (MeshWrapper != nullptr)
	{
		MeshWrapper->SetVisibility(true, true);
	}

	GetCharacterMovement()->GravityScale = DefaultGravity;
}

void AWorldPlayer::Hide()
{
	// Temp
	UCapsuleComponent* CapComponent = GetCapsuleComponent();
	CapComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	/*CapComponent->SetVisibility(false);
	CapComponent->SetComponentTickEnabled(false);
	CapComponent->SetActive(false);*/

	if (MeshWrapper != nullptr)
	{
		MeshWrapper->SetVisibility(false, true);
	}

	GetCharacterMovement()->GravityScale = 0.0f;
}

void AWorldPlayer::ChangeState(EStageState _State)
//void AWorldPlayer::ChangeState_Implementation(EStageState _State)
{
	CurStageState = _State;
}

void AWorldPlayer::DisableSpringArm()
{
	USpringArmComponent* CameraBoom = GetComponentByClass<USpringArmComponent>();
	if (CameraBoom)
	{
		CameraBoom->bEnableCameraLag = false;  // 카메라 지연 효과 제거
		CameraBoom->bEnableCameraRotationLag = false; // 회전 지연 효과 제거
		CameraBoom->bUsePawnControlRotation = false;  // 캐릭터 회전에 따라 카메라 회전 X
		CameraBoom->TargetArmLength = 0.0f; // SpringArm을 없애버림
	}
}

//void AWorldPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(AWorldPlayer, CurStageState);
//}
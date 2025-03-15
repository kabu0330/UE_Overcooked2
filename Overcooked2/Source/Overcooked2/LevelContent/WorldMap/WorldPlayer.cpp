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
	SetReplicates(true);

	DefaultGravity = GetCharacterMovement()->GravityScale;
}

void AWorldPlayer::BeginPlay()
{
	Super::BeginPlay();

	Hide();

	if (HasAuthority())
	{
		if (Controller != nullptr)	// Server side Server
		{
			Show_Client();
		}
	}
	else
	{
		if (Controller == nullptr)	// Client Side Server
		{
			Show_Client();
		}
	}

	SetActorLocation(START_LOC);

	ChangeState(EStageState::ShowStage1_1);
}

void AWorldPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Controller && HasAuthority() == false)
	{
		SetActorLocation(WorldPlayerLocation);
		UE_LOG(LogTemp, Warning, TEXT("################## Tick!! %f %f %f"), WorldPlayerLocation.X, WorldPlayerLocation.Y, WorldPlayerLocation.Z);
	}
}

void AWorldPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AWorldPlayer::OnMove(const FVector2D& _Vec)
{
	AddMovementInput(FVector(_Vec.X * 0.5f, _Vec.Y * 0.5f, 0.f));

	// TODO
	if (Controller && HasAuthority())
	{
		UpdateBusLocation(GetActorLocation());
	}
}

void AWorldPlayer::ChangeState(EStageState _State)
//void AWorldPlayer::ChangeState_Implementation(EStageState _State)
{
	CurStageState = _State;
}

void AWorldPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWorldPlayer, WorldPlayerLocation);
}

void AWorldPlayer::Show_Client_Implementation()
{
	Show();
}

void AWorldPlayer::Hide_Client_Implementation()
{
	Hide();
}

void AWorldPlayer::Show()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	//SetReplicates(true);
	GetCharacterMovement()->GravityScale = DefaultGravity;

	if (Controller)
	{
		APlayerController* PC = Cast<APlayerController>(Controller);
		if (PC)
		{
			EnableInput(PC);
		}
	}
}

void AWorldPlayer::Hide()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	//SetReplicates(false);
	GetCharacterMovement()->GravityScale = 0.0f;

	if (Controller)
	{
		APlayerController* PC = Cast<APlayerController>(Controller);
		if (PC)
		{
			DisableInput(PC);
		}
	}
}

void AWorldPlayer::UpdateBusLocation_Implementation(FVector _Loc)
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("################## HasAuth!! %f %f %f"), WorldPlayerLocation.X, WorldPlayerLocation.Y, WorldPlayerLocation.Z);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("################## NO HasAuth!! %f %f %f"), WorldPlayerLocation.X, WorldPlayerLocation.Y, WorldPlayerLocation.Z);
	}
	WorldPlayerLocation = _Loc;
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/WorldMap/WorldPlayer.h"
#include "LevelContent/WorldMap/WorldMapData.h"
#include "Global/OC2GameInstance.h"
#include "Global/OC2Global.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

FVector AWorldPlayer::WorldPlayerLocation = UWorldMapData::START_LOC;

// Sets default values
AWorldPlayer::AWorldPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

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

	SetActorLocation(UWorldMapData::START_LOC);
}

void AWorldPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Controller && HasAuthority() == false)	// Client Side Client
	{
		SetActorLocation(WorldPlayerLocation);
	}
}

void AWorldPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AWorldPlayer::OnMove(const FVector2D& _Vec)
{
	if (Controller && HasAuthority())
	{
		// Used Player's mapping context
		AddMovementInput(FVector(_Vec.Y * 0.5f, _Vec.X * -0.5f, 0.f));
		UpdateBusLocation(GetActorLocation());
	}
}

void AWorldPlayer::OnSelectMap()
{
	if (Controller && HasAuthority())
	{
		UOC2Global::GetOC2GameInstance(GetWorld())->StartGame();
	}
}

void AWorldPlayer::ToggleController(bool _IsOn)
{
	if (Controller == nullptr)
	{
		return;
	}

	APlayerController* PC = Cast<APlayerController>(Controller);
	if (PC == nullptr)
	{
		return;
	}

	if (_IsOn)
	{
		EnableInput(PC);
	}
	else
	{
		DisableInput(PC);
	}
}

void AWorldPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AWorldPlayer, WorldPlayerLocation);
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
}

void AWorldPlayer::Hide()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	//SetReplicates(false);
	GetCharacterMovement()->GravityScale = 0.0f;
}

void AWorldPlayer::UpdateBusLocation_Implementation(FVector _Loc)
{
	WorldPlayerLocation = _Loc;
}
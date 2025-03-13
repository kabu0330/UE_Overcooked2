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

	// Change Camera
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), TEXT("FocusCamera"), Actors);

	if (Actors.Num() > 0)
	{
		FocusCameraActor = Actors[0];
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		PlayerController->SetViewTargetWithBlend(FocusCameraActor, 0.0f);
	}

	InitParentSceneComp();
	Hide();

	if (GetWorld()->GetAuthGameMode())
	{
		ChangeState_Implementation(EStageState::ShowStage1_1);
	}
}

void AWorldPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurStageState == EStageState::ShowStage1_1)
	{
		RunStageShowing();
	}
	else if (CurStageState == EStageState::HideStage1_1)
	{
		RunStageHide();
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

void AWorldPlayer::RunStageShowing()
{
	FVector CurLoc = FocusCameraActor->GetActorLocation();
	FVector DestLoc(-320.f, 420.f, CurLoc.Z);	// Temp

	float Dist = FVector::Dist(DestLoc, CurLoc);
	if (Dist < 2.f)	// Temp
	{
		CurStageState = EStageState::HideStage1_1;
		UOC2GameInstance* GameInst = GetWorld()->GetGameInstance<UOC2GameInstance>();
		if (GameInst)
		{
			GameInst->SetCurStage(EOC2Stage::EOS_Sushi_1_2);	// Temp
		}
		FocusCameraActor->SetActorLocation(DestLoc);
		return;
	}

	FVector Dir = DestLoc - CurLoc;
	Dir.Normalize();
	Dir *= 2.f;
	FVector LocVec = CurLoc + Dir;
	FocusCameraActor->SetActorLocation(FVector(LocVec.X, LocVec.Y, CurLoc.Z));
}

void AWorldPlayer::RunStageHide()
{
	UOC2GameInstance* GameInst = GetWorld()->GetGameInstance<UOC2GameInstance>();
	if (!GameInst)
	{
		return;
	}

	if (GameInst->GetCurStage() != EOC2Stage::EOS_Sushi_1_1)		// Temp
	{
		return;
	}

	FVector CurLoc = FocusCameraActor->GetActorLocation();
	FVector DestLoc(-100.f, 150.f, CurLoc.Z);	// Temp

	float Dist = FVector::Dist(DestLoc, CurLoc);
	if (Dist < 2.f)	// Temp
	{
		CurStageState = EStageState::None;
		//GameInst->SetStageState(4);	// Temp
		FocusCameraActor->SetActorLocation(DestLoc);
		Show();

		// Change Camera
		UCameraComponent* CurrentCameraComponent = FindComponentByClass<UCameraComponent>();
		if (CurrentCameraComponent)
		{
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
			PlayerController->SetViewTargetWithBlend(this, 0.0f);
		}

		return;
	}

	FVector Dir = DestLoc - CurLoc;
	Dir.Normalize();
	Dir *= 2.f;
	FVector LocVec = CurLoc + Dir;
	FocusCameraActor->SetActorLocation(FVector(LocVec.X, LocVec.Y, CurLoc.Z));
}

void AWorldPlayer::ChangeState_Implementation(EStageState _State)
{
	CurStageState = _State;
}

void AWorldPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWorldPlayer, CurStageState);
}
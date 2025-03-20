// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/WorldMap/WorldMapDialogue.h"
#include "LevelContent/WorldMap/WorldPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Global/OC2Global.h"
#include "Materials/MaterialInstanceConstant.h"
#include "GameFramework/SpringArmComponent.h"	// Temp

AWorldMapDialogue::AWorldMapDialogue()
{
	PrimaryActorTick.bCanEverTick = true;

	// TODO: literal
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = SceneComponent;

	DialogMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("DialogMesh");
	DialogMeshComponent->SetRelativeRotation(FQuat::MakeFromEuler({ 90.f, 0.f, 90.f }));
	DialogMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DialogMeshComponent->SetupAttachment(RootComponent);

	CaptureMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("CaptureMesh");
	CaptureMeshComponent->SetRelativeScale3D(FVector{ 0.8f, 0.5f, 1.f });
	CaptureMeshComponent->SetRelativeLocation(FVector{ 0.f, 0.f, 14.f });
	CaptureMeshComponent->SetRelativeRotation(FQuat::MakeFromEuler({ 90.f, 0.f, 90.f }));
	CaptureMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CaptureMeshComponent->SetupAttachment(RootComponent);

	// Temp
	{
		FString MatName = "Plane";
		FString MatPath = "";

		UOC2Global::GetAssetPackageName(UStaticMesh::StaticClass(), MatName, MatPath);
		if (!MatPath.IsEmpty())
		{
			ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(*MatPath);
			if (!Asset.Succeeded())
			{
				UE_LOG(LogTemp, Fatal, TEXT("Check the mesh: %s"), *MatName);
			}

			DialogMeshComponent->SetStaticMesh(Asset.Object);
			CaptureMeshComponent->SetStaticMesh(Asset.Object);
		}
	}

	{
		FString MatName = "MI_StageDialogue";
		FString MatPath = "";

		UOC2Global::GetAssetPackageName(UMaterialInstanceConstant::StaticClass(), MatName, MatPath);
		if (!MatPath.IsEmpty())
		{
			ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> Asset(*MatPath);
			if (!Asset.Succeeded())
			{
				UE_LOG(LogTemp, Fatal, TEXT("Check the material instance: %s"), *MatName);
			}

			DialogMeshComponent->SetMaterial(0, Asset.Object);
		}
	}

	// Temp
	{
		FString MatName = "MI_StageCapture1_1";
		FString MatPath = "";

		UOC2Global::GetAssetPackageName(UMaterialInstanceConstant::StaticClass(), MatName, MatPath);
		if (!MatPath.IsEmpty())
		{
			ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> Asset(*MatPath);
			if (!Asset.Succeeded())
			{
				UE_LOG(LogTemp, Fatal, TEXT("Check the material instance: %s"), *MatName);
			}

			CaptureMeshComponent->SetMaterial(0, Asset.Object);
		}
	}
}

void AWorldMapDialogue::BeginPlay()
{
	Super::BeginPlay();

	InitCameraToLook();

	// Temp
	SetActorLocation(FVector(0.f, 0.f, 200.f));
}

void AWorldMapDialogue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWorldMapDialogue::SetStage(int _Stage)
{
	if (_Stage < 0)
	{
		return;
	}

	FString MatName = "";
	FString MatPath = "";

	// Temp
	if (_Stage == 1)
	{
		MatName = "MI_StageCapture1_1";
	}
}

void AWorldMapDialogue::InitCameraToLook()
{
	UWorld* Level = GetWorld();
	if (Level == nullptr)
	{
		return;
	}

	APlayerController* PC = Level->GetFirstPlayerController();
	if (PC == nullptr)
	{
		return;
	}

	APawn* PlayerPawn = PC->GetPawn();
	if (PlayerPawn == nullptr)
	{
		return;
	}

	AWorldPlayer* Player = Cast<AWorldPlayer>(PlayerPawn);
	if (Player == nullptr)
	{
		return;
	}
	
	USpringArmComponent* SpringArm = Player->GetComponentByClass<USpringArmComponent>();
	if (SpringArm == nullptr)
	{
		return;
	}

	FVector CurLoc = Player->GetActorLocation();
	FVector CamLoc = SpringArm->GetComponentLocation();
	FRotator LookAtRotation = (CamLoc - CurLoc).Rotation();

	LookAtRotation.Pitch = 0.0f;
	LookAtRotation.Roll = 0.0f;

	SetActorRelativeRotation(LookAtRotation);
}

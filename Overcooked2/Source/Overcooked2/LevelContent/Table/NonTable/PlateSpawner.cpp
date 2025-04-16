// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/NonTable/PlateSpawner.h"
#include "LevelContent/Cook/Plate.h"
#include <Net/UnrealNetwork.h>
#include <Global/OC2Global.h>
#include <Global/Data/OC2GlobalData.h>

APlateSpawner::APlateSpawner()
{
	bReplicates = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	PlateMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlateMeshComponent"));
	PlateMeshComponent->SetupAttachment(RootComponent);
}

void APlateSpawner::BeginPlay()
{
	Super::BeginPlay();
}

void APlateSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlateSpawner::SetPlate(class APlate* Plate)
{
	if (nullptr != CookingPtrPlate)
	{
		CookingPtrPlate->StackPlate(Plate);
	}
	else
	{
		CookingPtrPlate = Plate;
		CookingPtrPlate->AttachToComponent(ComponentForCooking, FAttachmentTransformRules::KeepRelativeTransform);
		Plate->SetActorRelativeLocation(FVector::ZeroVector);
	}
}

ACooking* APlateSpawner::Interact(AActor* ChefActor)
{
	CookingPtr = Cast<ACooking>(CookingPtrPlate);
	CookingPtrPlate = nullptr;
	return CookingPtr;
}

void APlateSpawner::PlaceItem(ACooking* ReceivedCooking)
{
	if (nullptr == ReceivedCooking)
	{
		return;
	}
	ACooking* TempCooking = ReceivedCooking;
}

void APlateSpawner::SetPlateMesh()
{
	FVector InitPos = GetActorLocation() + FVector(0, 0, 50);
	switch (PlateNum)
	{
	case 0:
	{
		PlateMeshComponent->SetStaticMesh(nullptr);
		break;
	}
	case 1:
	{
		UStaticMesh* NewMesh = UOC2GlobalData::GetResourceStaticMesh(GetWorld(), TEXT("SinglePlate"));
		PlateMeshComponent->SetStaticMesh(NewMesh);

		PlateMeshComponent->SetRelativeLocation(InitPos + FVector(0, 0, 0));
		break;
	}
	case 2:
	{
		UStaticMesh* NewMesh = UOC2GlobalData::GetResourceStaticMesh(GetWorld(), TEXT("DoublePlate"));
		PlateMeshComponent->SetStaticMesh(NewMesh);

		PlateMeshComponent->SetRelativeLocation(InitPos + FVector(0, 0, 20));
		break;
	}
	case 3:
	{
		UStaticMesh* NewMesh = UOC2GlobalData::GetResourceStaticMesh(GetWorld(), TEXT("TriplePlate"));
		PlateMeshComponent->SetStaticMesh(NewMesh);

		PlateMeshComponent->SetRelativeLocation(InitPos + FVector(0, 0, 0));
		break;
	}
	case 4:
	{
		UStaticMesh* NewMesh = UOC2GlobalData::GetResourceStaticMesh(GetWorld(), TEXT("FullPlate"));
		PlateMeshComponent->SetStaticMesh(NewMesh);

		PlateMeshComponent->SetRelativeLocation(InitPos + FVector(0, 0, 60));
		break;
	}
	default:
		break;
	}
}

void APlateSpawner::AddPlate_Implementation(int Number)
{
	if (true == HasAuthority())
	{
		PlateNum += Number;
		if (0 > PlateNum)
		{
			PlateNum = 0;
		}
		if (4 < PlateNum)
		{
			PlateNum = 4;
		}
		SetPlateMesh();
		SetMaterialTextrue();
	}
}

void APlateSpawner::InitPlateNum_Implementation()
{
	PlateNum = 0;
	SetPlateMesh();
	SetMaterialTextrue();
}

void APlateSpawner::OnRep_SetPlateMesh()
{
	SetPlateMesh();
	SetMaterialTextrue();
}

void APlateSpawner::SetMaterialTextrue()
{
	for (int i = 0; i < PlateMeshComponent->GetNumMaterials(); i++)
	{
		UMaterialInstanceDynamic* MaterialInstanceDynamic = Cast<UMaterialInstanceDynamic>(PlateMeshComponent->GetMaterial(i));
		if (nullptr != MaterialInstanceDynamic)
		{
			MaterialInstanceDynamic->SetTextureParameterValue(FName(TEXT("DiffuseColorMap")), DirtyTexture);
			MaterialInstanceDynamic->SetScalarParameterValue(FName("DiffuseAdd"), 1.0f);
			PlateMeshComponent->SetMaterial(i, MaterialInstanceDynamic);
			return;
		}
	}
}

void APlateSpawner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlateSpawner, PlateMeshComponent);
	DOREPLIFETIME(APlateSpawner, PlateNum);
}


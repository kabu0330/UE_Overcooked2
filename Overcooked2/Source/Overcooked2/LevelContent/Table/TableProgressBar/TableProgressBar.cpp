// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/TableProgressBar/TableProgressBar.h"



ATableProgressBar::ATableProgressBar()
{
	bReplicates = true;

	//ComponentForProgress = CreateDefaultSubobject<USceneComponent>("Attachment");
	//ComponentForProgress->SetupAttachment(RootComponent);

	BackGroundMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("BackGround");
	BackGroundMeshComponent->SetupAttachment(RootComponent);
	ProgressMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Progress");
	ProgressMeshComponent->SetupAttachment(RootComponent);
}

void ATableProgressBar::BeginPlay()
{
	Super::BeginPlay();

	SetComponentMesh();
}

void ATableProgressBar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//UpdateLength();

	if (true == bDestroy)
	{
		this->Destroy();
	}
}

void ATableProgressBar::SetComponentMesh()
{
	UStaticMesh* Mesh1 = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Resources/LevelResource/TableResource/TimerBar/SM_TimerBar"));
	BackGroundMeshComponent->SetStaticMesh(Mesh1);
	FVector Location = BackGroundMeshComponent->GetComponentLocation();

	UStaticMesh* Mesh2 = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Resources/LevelResource/TableResource/TimerBar/SM_Progress"));
	ProgressMeshComponent->SetStaticMesh(Mesh2);
	ProgressMeshComponent->SetRelativeLocation(Location + FVector(0.0, 100.0, 0.0));
	ProgressMeshComponent->AddWorldRotation({ 0.0, 0.0, 120.0 });
}

void ATableProgressBar::UpdateLength()
{
	FTransform Transform = ProgressMeshComponent->GetComponentTransform();
	FVector Length = Transform.GetScale3D();
	Length.X *= 2.0;
	Transform = FTransform(Length);
	ProgressMeshComponent->SetWorldTransform(Transform);
}


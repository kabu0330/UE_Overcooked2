// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Title/DrivingCar.h"
#include "NiagaraComponent.h"

// Sets default values
ADrivingCar::ADrivingCar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);


	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);
	NiagaraComponent->SetAutoActivate(true);
}

void ADrivingCar::SetNiagaraOffset(FTransform Trans)
{
	NiagaraComponent->SetRelativeLocation(Trans.GetLocation());
	NiagaraComponent->SetRelativeRotation(Trans.GetRotation());
	NiagaraComponent->SetRelativeScale3D(Trans.GetScale3D());
}

void ADrivingCar::CreateDynamicMaterial()
{
	TArray<UMeshComponent*> MeshComponents;
	GetComponents<UMeshComponent>(MeshComponents);

	if (true == MeshComponents.IsEmpty())
	{
		return;
	}

	for (UMeshComponent* Mesh : MeshComponents)
	{
		if (nullptr == Mesh)
		{
			continue;
		}

		for (int i = 0; i < 1/*Mesh->GetNumMaterials()*/; i++)
		{
			UMaterialInterface* Material = Mesh->GetMaterials()[i];
			if (nullptr != Material)
			{
				UMaterialInstanceDynamic* CastMaterial = Cast<UMaterialInstanceDynamic>(Material);
				if (nullptr != CastMaterial) // 이미 다이나믹 머티리얼이 생성되어 있다면 또 만들지 않는다.
				{
					continue;
				}
				UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
				if (nullptr != DynamicMaterial)
				{
					MaterialInstanceDynamic = DynamicMaterial;
					Mesh->SetMaterial(i, DynamicMaterial);
				}
			}
		}
	}
}

void ADrivingCar::ChangeColor(FVector4 Color)
{
	MaterialInstanceDynamic->SetVectorParameterValue(FName("Tint"), Color);
}

// Called when the game starts or when spawned
void ADrivingCar::BeginPlay()
{
	Super::BeginPlay();
	
	CreateDynamicMaterial();
	InitPos = GetActorLocation();

	if (nullptr != NiagaraComponent)
	{
		if (nullptr != Niagara)
		{
			NiagaraComponent->SetAsset(Niagara);
			NiagaraComponent->Activate(true);
		}
	}
}

// Called every frame
void ADrivingCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeElapsed += DeltaTime;
	if (TimeElapsed >= LoopCooldown)
	{
		TimeElapsed = 0.0f;
		SetActorLocation(InitPos);
	}
}


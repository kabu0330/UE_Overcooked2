// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/GameFramework/OC2Actor.h"
#include "Net/UnrealNetwork.h"

AOC2Actor::AOC2Actor()
{
	PrimaryActorTick.bCanEverTick = true;

	SecenComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SecenComponent"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
	
	//StaticMeshComponent->SetupAttachment(RootComponent);
}

void AOC2Actor::InitOC2Actor()
{
}

void AOC2Actor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOC2Actor, SecenComponent);
	DOREPLIFETIME(AOC2Actor, StaticMeshComponent);
}

void AOC2Actor::BeginPlay()
{
	Super::BeginPlay();

}

void AOC2Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOC2Actor::ApplyMaterialHighlight()
{
	if (nullptr == StaticMeshComponent)
	{
		return;
	}

	float HighlightValue = 5.0f;
	UMeshComponent* Mesh = nullptr;
	if (nullptr == (Mesh = FindComponentByClass<USkeletalMeshComponent>()))
	{
		Mesh = StaticMeshComponent;
	}

	for (int i = 0; i < Mesh->GetNumMaterials(); i++)
	{
		if (Mesh->GetMaterials().IsValidIndex(i) && Mesh->GetMaterials()[i] != nullptr)
		{
			UMaterialInstanceDynamic* ExistingDynamicMaterial = Cast<UMaterialInstanceDynamic>(Mesh->GetMaterials()[i]);
			if (ExistingDynamicMaterial)
			{
				// 이미 존재하는 다이나믹 머티리얼이 있으면 값을 변경
				Mesh->GetMaterial(0)->GetScalarParameterValue(FName("DiffuseColorMapWeight"), DiffuseColorMapWeight);
				ExistingDynamicMaterial->SetScalarParameterValue(FName("DiffuseColorMapWeight"), HighlightValue);
				continue;
			}
		}

		// 1. 메시의 머티리얼을 모두 가져온다.
		UMaterialInterface* Material = Mesh->GetMaterial(i);
		if (nullptr != Material)
		{
			// 2. 동적 머티리얼 인스턴스를 생성한다.
			// 기존 머티리얼은 스태틱이라 수정할 수 없다.
			UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);

			if (nullptr != DynamicMaterial)
			{
				// 3. 기존 머티리얼을 먼저 저장하고
				Material->GetScalarParameterValue(FName("DiffuseColorMapWeight"), DiffuseColorMapWeight);
				//Materials.Add(Material);

				// 4. 다이나믹 머티리얼로 교체한다.
				DynamicMaterial->SetScalarParameterValue(FName("DiffuseColorMapWeight"), HighlightValue);
				Mesh->SetMaterial(i, DynamicMaterial);
			}
		}
	}
	bIsHighlighted = true;
}

void AOC2Actor::RestoreMaterial()
{
	UMeshComponent* Mesh = nullptr;
	if (nullptr == (Mesh = GetComponentByClass<USkeletalMeshComponent>()))
	{
		Mesh = StaticMeshComponent;
	}

	if (Mesh != nullptr)
	{
		for (int i = 0; i < Mesh->GetNumMaterials(); i++)
		{
			if (Mesh->GetMaterials().IsValidIndex(i) && Mesh->GetMaterials()[i] != nullptr)
			{
				UMaterialInstanceDynamic* ExistingDynamicMaterial = Cast<UMaterialInstanceDynamic>(Mesh->GetMaterials()[i]);
				if (ExistingDynamicMaterial)
				{
					// 이미 존재하는 다이나믹 머티리얼이 있으면 값을 변경
					ExistingDynamicMaterial->SetScalarParameterValue(FName("DiffuseColorMapWeight"), DiffuseColorMapWeight);
					continue;
				}
			}

			UMaterialInterface* Material = Materials[i];
			if (nullptr != Material)
			{
				UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
				if (nullptr != DynamicMaterial)
				{
					DynamicMaterial->SetScalarParameterValue(FName("DiffuseColorMapWeight"), DiffuseColorMapWeight);
					Mesh->SetMaterial(i, DynamicMaterial);
				}
			}
		}
	}
	

	// 2. 저장해둔 머티리얼은 초기화
	Materials.Empty();
	bIsHighlighted = false;
}
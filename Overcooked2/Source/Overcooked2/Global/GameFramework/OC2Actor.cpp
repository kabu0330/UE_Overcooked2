// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/GameFramework/OC2Actor.h"

AOC2Actor::AOC2Actor()
{
	PrimaryActorTick.bCanEverTick = true;

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

	for (int i = 0; i < StaticMeshComponent->GetNumMaterials(); i++)
	{
		StaticMeshComponent->SetMaterial(i, nullptr);
	}

	float HighlightValue = 5.0f;
	for (int i = 0; i < StaticMeshComponent->GetNumMaterials(); i++)
	{
		// 1. 메시의 머티리얼을 모두 가져온다.
		UMaterialInterface* Material = StaticMeshComponent->GetMaterial(i);
		if (nullptr != Material)
		{
			// 2. 동적 머티리얼 인스턴스를 생성한다.
			// 기존 머티리얼은 스태틱이라 수정할 수 없다.
			UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);

			if (nullptr != DynamicMaterial)
			{
				// 3. 기존 머티리얼을 먼저 저장하고
				Material->GetScalarParameterValue(FName("DiffuseColorMapWeight"), DiffuseColorMapWeight);
				Materials.Add(Material);

				// 4. 다이나믹 머티리얼로 교체한다.
				DynamicMaterial->SetScalarParameterValue(FName("DiffuseColorMapWeight"), HighlightValue);
				StaticMeshComponent->SetMaterial(i, DynamicMaterial);
			}
		}
	}
	bIsHighlighted = true;
}

void AOC2Actor::RestoreMaterial()
{
	if (true == Materials.IsEmpty())
	{
		return;
	}

	// 1. 머티리얼을 원래대로 되돌리고
	for (int i = 0; i < StaticMeshComponent->GetNumMaterials(); i++)
	{
		UMaterialInterface* Material = Materials[i];
		if (nullptr != Material)
		{
			UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
			if (nullptr != DynamicMaterial)
			{
				DynamicMaterial->SetScalarParameterValue(FName("DiffuseColorMapWeight"), DiffuseColorMapWeight);
				StaticMeshComponent->SetMaterial(i, DynamicMaterial);
			}
		}
	}

	// 2. 저장해둔 머티리얼은 초기화
	Materials.Empty();
	bIsHighlighted = false;
}
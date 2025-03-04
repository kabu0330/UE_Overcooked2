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
	ApplyMaterialHighlight(StaticMeshComponent);
}

void AOC2Actor::ApplyMaterialHighlight(UMeshComponent* Mesh)
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
		// 1. �޽��� ��Ƽ������ ��� �����´�.
		UMaterialInterface* Material = StaticMeshComponent->GetMaterial(i);
		if (nullptr != Material)
		{
			// 2. ���� ��Ƽ���� �ν��Ͻ��� �����Ѵ�.
			// ���� ��Ƽ������ ����ƽ�̶� ������ �� ����.
			UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);

			if (nullptr != DynamicMaterial)
			{
				// 3. ���� ��Ƽ������ ���� �����ϰ�
				Material->GetScalarParameterValue(FName("DiffuseColorMapWeight"), DiffuseColorMapWeight);
				Materials.Add(Material);

				// 4. ���̳��� ��Ƽ����� ��ü�Ѵ�.
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

	// 1. ��Ƽ������ ������� �ǵ�����
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

	// 2. �����ص� ��Ƽ������ �ʱ�ȭ
	Materials.Empty();
	bIsHighlighted = false;
}
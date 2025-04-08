// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/GameFramework/OC2Actor.h"
#include "Net/UnrealNetwork.h"
#include <Components/WidgetComponent.h>

AOC2Actor::AOC2Actor()
{
	PrimaryActorTick.bCanEverTick = true;

	SecenComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SecenComponent"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	bReplicates = true;
	
	//StaticMeshComponent->SetupAttachment(RootComponent);
}

void AOC2Actor::InitOC2Actor()
{
}

void AOC2Actor::SetHighlight_Implementation(bool Value)
{
	bIsHighlighted = Value;
	OnRep_Highlight();
}

void AOC2Actor::OnRep_Highlight()
{
	if (bIsHighlighted)
	{
		ApplyMaterialHighlight();
	}
	else
	{
		RestoreMaterial();
	}
}

void AOC2Actor::RequestOC2ActorDestroy()
{
	if (false == HasAuthority())  // 클라이언트에서 실행 중인지 확인
	{
		Server_DestroyOC2Actor();  // 서버에게 삭제 요청
	}
	else
	{
		Destroy();  // 이미 서버라면 직접 삭제
	}
}

void AOC2Actor::Server_DestroyOC2Actor_Implementation()
{
}

bool AOC2Actor::Server_DestroyOC2Actor_Validate()
{
	return true;
}

void AOC2Actor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOC2Actor, SecenComponent);
	DOREPLIFETIME(AOC2Actor, StaticMeshComponent);
	DOREPLIFETIME(AOC2Actor, bIsHighlighted);
}

void AOC2Actor::BeginPlay()
{
	Super::BeginPlay();

	TArray<UMeshComponent*> MeshComponents;
	GetComponents<UMeshComponent>(MeshComponents);

	if (true == MeshComponents.IsEmpty())
	{
		return;
	}

	int Count = 0;

	for (UMeshComponent* Mesh : MeshComponents)
	{
		if (nullptr == Mesh)
		{
			continue;
		}
		if (nullptr != Cast<UWidgetComponent>(Mesh))
		{
			continue;
		}

		for (int i = 0; i < Mesh->GetNumMaterials(); i++)
		{
			// 최초 하이라이트
			UMaterialInterface* Material = Mesh->GetMaterials()[i];
			if (nullptr != Material)
			{
				UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
				if (nullptr != DynamicMaterial)
				{
					float Temp;
					Mesh->GetMaterials()[i]->GetScalarParameterValue(FName("DiffuseAdd"), Temp);
					DiffuseColorMapWeights.Add(Temp);
					Mesh->SetMaterial(i, DynamicMaterial);
				}
			}
		}
	}

}

void AOC2Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOC2Actor::ApplyMaterialHighlight()
{
	TArray<UMeshComponent*> MeshComponents;
	GetComponents<UMeshComponent>(MeshComponents);

	if (true == MeshComponents.IsEmpty())
	{
		return;
	}

	int Count = 0;

	for (UMeshComponent* Mesh : MeshComponents)
	{
		if (nullptr == Mesh)
		{
			continue;
		}

		for (int i = 0; i < Mesh->GetNumMaterials(); i++)
		{
			// 다이나믹 머티리얼로 교체된 이후인 경우
			if (true == Mesh->GetMaterials().IsValidIndex(i) && nullptr != Mesh->GetMaterials()[i])
			{
				UMaterialInstanceDynamic* ExistingDynamicMaterial = Cast<UMaterialInstanceDynamic>(Mesh->GetMaterials()[i]);
				if (nullptr != ExistingDynamicMaterial)
				{
					Mesh->GetMaterials()[i]->GetScalarParameterValue(FName("DiffuseAdd"), DiffuseColorMapWeights[Count]);
					ExistingDynamicMaterial->SetScalarParameterValue(FName("DiffuseAdd"), HighlightValue);
					Mesh->SetMaterial(i, ExistingDynamicMaterial);
					++Count;
					continue;
				}
			}
			
			//// 최초 하이라이트
			//UMaterialInterface* Material = Mesh->GetMaterials()[i];
			//if (nullptr != Material)
			//{
			//	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
			//	if (nullptr != DynamicMaterial)
			//	{
			//		float Temp;
			//		Mesh->GetMaterials()[i]->GetScalarParameterValue(FName("DiffuseAdd"), Temp);
			//		DiffuseColorMapWeights.Add(Temp);
			//		DynamicMaterial->SetScalarParameterValue(FName("DiffuseAdd"), HighlightValue);
			//		Mesh->SetMaterial(i, DynamicMaterial);
			//		++Count;
			//	}
			//}
		}
	}

	bIsHighlighted = true;
}

void AOC2Actor::RestoreMaterial()
{
	TArray<UMeshComponent*> MeshComponents;
	GetComponents<UMeshComponent>(MeshComponents);

	if (true == MeshComponents.IsEmpty())
	{
		return;
	}

	int Count = 0;
	for (UMeshComponent* Mesh : MeshComponents)
	{
		if (nullptr == Mesh)
		{
			continue;
		}

		for (int i = 0; i < Mesh->GetNumMaterials(); i++)
		{
			if (true == Mesh->GetMaterials().IsValidIndex(i) && nullptr != Mesh->GetMaterials()[i])
			{
				UMaterialInstanceDynamic* ExistingDynamicMaterial = Cast<UMaterialInstanceDynamic>(Mesh->GetMaterials()[i]);
				if (nullptr != ExistingDynamicMaterial)
				{
					ExistingDynamicMaterial->SetScalarParameterValue(FName("DiffuseAdd"), DiffuseColorMapWeights[Count]);
					Mesh->SetMaterial(i, ExistingDynamicMaterial);
					++Count;
				}
			}
		}
	}

	// 2. 저장해둔 머티리얼은 초기화
	bIsHighlighted = false;
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OC2CharacterTestObject.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AOC2CharacterTestObject::AOC2CharacterTestObject()
{
}

void AOC2CharacterTestObject::SetIngredient(EIngredientType Type)
{
	IngredientType = Type;
}

void AOC2CharacterTestObject::SetPhysics_Implementation(bool Value)
{
	StaticMeshComponent->SetSimulatePhysics(Value);
	if (Value)
	{
		StaticMeshComponent->SetCollisionProfileName(TEXT("Interactable"));
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AOC2CharacterTestObject::BeginPlay()
{
	Super::BeginPlay();
	// 2. Setting
	UOC2GameInstance* GameInst = Cast<UOC2GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	FName Name = GameInst->GetIngredientDataTableRowName(IngredientType);

	// 1. 해당 재료 타입의 데이터 행 추출
	// FIngredientDataRow IngredientDataTable
	IngredientDataTable = &GameInst->GetIngredientDataRow(Name);

	StaticMeshComponent->SetStaticMesh(IngredientDataTable->BaseMesh);
	StaticMeshComponent->SetCollisionProfileName("Interactable");
	StaticMeshComponent->SetSimulatePhysics(true);

	IngredientType = IngredientDataTable->IngredientType;
	CurIngredientState = IngredientDataTable->StateRows[0].PrevIngredientState;

	// 3. Offset
	FVector Location = IngredientDataTable->Location;
	FRotator Rotation = IngredientDataTable->Rotation;
	AddActorLocalOffset(Location);
	SetActorRelativeRotation(Rotation);

}

void AOC2CharacterTestObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOC2CharacterTestObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOC2CharacterTestObject, IngredientType);
}

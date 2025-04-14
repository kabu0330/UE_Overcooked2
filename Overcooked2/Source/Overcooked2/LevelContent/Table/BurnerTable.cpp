// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/BurnerTable.h"
#include <Character/OC2Character.h>
#include <LevelContent/Cook/Pot.h>
#include <Global/GameMode/OC2GameMode.h>
#include <Global/OC2Enum.h>

ABurnerTable::ABurnerTable()
{
	FlameMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Flame");
	FlameMeshComponent->SetupAttachment(RootComponent);
}

void ABurnerTable::BeginPlay()
{
	Super::BeginPlay();

	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Resources/LevelResource/TableResource/BurnerTable/SM_CookerFlame"));
	FlameMeshComponent->SetStaticMesh(Mesh);
	FlameMeshComponent->SetHiddenInGame(true);

	if (bSpawnWhenGameStarted && HasAuthority())
	{
		RequestSpawnPot();
	}
}

void ABurnerTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (nullptr != CookingPtr)
	{
		APot* TempPot = Cast<APot>(CookingPtr);
		if (true == TempPot->IsRiceInPot())
		{
			FlameMeshComponent->SetHiddenInGame(false);
		}
		else
		{
			FlameMeshComponent->SetHiddenInGame(true);
		}
	}
	else
	{
		FlameMeshComponent->SetHiddenInGame(true);
	}
}

ACooking* ABurnerTable::Interact(AActor* ChefActor)
{
	AOC2Character* Chef = Cast<AOC2Character>(ChefActor);
	ACooking* TempCooking = nullptr;

	if (CookingPtr != nullptr && false == Chef->IsHolding()) // 음식 있음, 셰프 빈 손
	{
		TempCooking = CookingPtr;
		CookingPtr = nullptr;
		return TempCooking;
	}
	else
	{
		return CookingPtr;
	}
}

void ABurnerTable::PlaceItem(ACooking* Item)
{
	if (nullptr == Item)
	{
		return;
	}
	ACooking* TempCooking = Item;
	AIngredient* Ingredient = Cast<AIngredient>(Item);

	if (true == TempCooking->IsCookingType(ECookingType::ECT_POT))
	{
		CookingPtr = TempCooking;
		FRotator Rotation = FRotator(0, 0, 0);

		CookingPtr->AttachToComponent(ComponentForCooking, FAttachmentTransformRules::KeepRelativeTransform);
		CookingPtr->SetActorLocation(ComponentForCooking->GetComponentLocation());
		CookingPtr->GetStaticMeshComponent()->SetRelativeRotation(Rotation);

		CookingPtr->SetCookingTable_Implementation(this);
		APot* TempPot = Cast<APot>(CookingPtr);

		//냄비의 음식물이 Boilable이면 바로 BoilThePot();
	}
	else if (nullptr != CookingPtr && nullptr != Ingredient)
	{
		if (EIngredientType::EIT_RICE == Ingredient->GetIngredientType() && EIngredientState::EIS_BOILABLE == Ingredient->GetCurIngredientState())
		{
			APot* Pot = Cast<APot>(CookingPtr);
			if (nullptr != Pot)
			{
				Pot->Add(Ingredient);
			}
		}
	}
}

void ABurnerTable::BoilThePot()
{

}

void ABurnerTable::RequestSpawnPot_Implementation()
{
	auto GameMode = Cast<AOC2GameMode>(GetWorld()->GetAuthGameMode());
	APot* Pot = nullptr;
	if (GameMode)
	{
		Pot = GameMode->SpawnPotActor(EPotState::IDLE);
	}
	Pot->AttachToChef(this);
	FRotator Rotation = FRotator(0, 90, 0);
	Pot->AddActorLocalRotation(Rotation);
	Pot->SetActorLocation(ComponentForCooking->GetComponentLocation());
	CookingPtr = Pot;
}

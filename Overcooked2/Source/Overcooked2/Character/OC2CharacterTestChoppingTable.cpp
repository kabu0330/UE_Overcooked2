// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OC2CharacterTestChoppingTable.h"
#include "OC2Character.h"

// Sets default values
AOC2CharacterTestChoppingTable::AOC2CharacterTestChoppingTable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOC2CharacterTestChoppingTable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOC2CharacterTestChoppingTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ACooking* AOC2CharacterTestChoppingTable::Interact(AActor* ChefActor)
{
	AOC2Character* Character = Cast<AOC2Character>(ChefActor);

	if (CurCook == nullptr)
	{
		return nullptr;
	}
	else
	{
		ACooking* Ret = CurCook;
		CurCook = nullptr;
		return Ret;
	}
}

void AOC2CharacterTestChoppingTable::PlaceItem(ACooking* Cook)
{
	CurCook = Cook;
	CurCook->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	CurCook->SetActorLocation(GetActorLocation() + FVector(0,0,100));
}



void AOC2CharacterTestChoppingTable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
}


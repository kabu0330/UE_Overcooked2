// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/NonTable/GarbageCan.h"
#include "Kismet/GameplayStatics.h"
#include "Global/GameMode/OC2GameMode.h"

AGarbageCan::AGarbageCan()
{

}

void AGarbageCan::BeginPlay()
{
	Super::BeginPlay();
}

void AGarbageCan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDestroyGarbage == true)
	{
		Timer -= DeltaTime;
		// ...쓰레기 회전시키기...
		FRotator Rotation = { 0.0, 0.0, 90.0 };
		//FTransform Location = { 0.0, 0.0, -10.0 };
		CookingPtr->AddActorWorldRotation(Rotation * DeltaTime);
		FVector Scale = CookingPtr->GetActorScale3D();
		CookingPtr->SetActorScale3D(Scale * 0.99);
		//Garbage->AddActorWorldTransform(Location);
	}

	if (Timer < 0)
	{
		//DestroyIngredient();
	}
}

void AGarbageCan::PlaceItem(ACooking* Item)
{
	// Ingredient만 없애기. Plate의 경우 AOC2Character가 담당
	if (true == Item->IsCookingType(ECookingType::ECT_INGREDIENT))
	{
		CookingPtr = Item;
		CookingPtr->AttachToComponent(ComponentForCooking, FAttachmentTransformRules::KeepRelativeTransform);
		CookingPtr->SetActorLocation(ComponentForCooking->GetComponentLocation());

		bDestroyGarbage = true;
		Timer = 2.0f;
	}
}

void AGarbageCan::DestroyIngredient()
{
	AOC2GameMode* GM = Cast<AOC2GameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->DestroyOC2Actor(CookingPtr);
		CookingPtr = nullptr;
		bDestroyGarbage = false;
	}
}
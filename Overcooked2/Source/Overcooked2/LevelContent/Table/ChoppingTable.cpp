// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/ChoppingTable.h"
#include <Character/OC2Character.h>
#include <Global/OC2Enum.h>
#include "LevelContent/Table/TableProgressBar/TableProgressBar.h"
#include "Global/GameMode/OC2GameMode.h"

AChoppingTable::AChoppingTable()
{
	ComponentForProgressBar = CreateDefaultSubobject<USceneComponent>("ProgressBar");
	ComponentForProgressBar->SetupAttachment(RootComponent);
}

void AChoppingTable::BeginPlay()
{
	Super::BeginPlay();
}

void AChoppingTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (true == bTimerActivated)
	{
		TimerUpdate(DeltaTime);

		if (Timer < 0)
		{
			bChoppingDone = true;
		}
	}

	if (bChoppingDone == true)
	{
		ChoppingIsDone();
	}
}

ACooking* AChoppingTable::Interact(AActor* ChefActor)
{
	ChefPtr = Cast<AOC2Character>(ChefActor);
	ACooking* TempCooking = nullptr;

	if (CookingPtr != nullptr && false == ChefPtr->IsHolding()) // À½½Ä ÀÖÀ½, ¼ÎÇÁ ºó ¼Õ
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

void AChoppingTable::ChopIngredient(AActor* ChefActor)
{
	ChefPtr = Cast<AOC2Character>(ChefActor);

	if (nullptr != ChefActor && nullptr != CookingPtr)
	{
		if (true == CookingPtr->IsCookingType(ECookingType::ECT_INGREDIENT))
		{
			AIngredient* PlacedIngredient = Cast<AIngredient>(CookingPtr);

			//if (true == PlacedIngredient->IsChoppable())
			{
				ChefPtr->Chopping(true);

				Timer = 2.0f;
				bTimerActivated = true;
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Magenta, "Chopping...");

				ProgressBar = GetWorld()->SpawnActor<ATableProgressBar>();
				ProgressBar->SetActorLocation(ComponentForProgressBar->GetComponentLocation());
				FVector Scale = CookingPtr->GetActorScale3D();
				ProgressBar->SetActorScale3D(Scale * 0.3);
				ProgressBar->AddActorWorldRotation({ 0.0, 00.0, -60.0 });
			}
		}
	}
}

void AChoppingTable::ChoppingIsDone()
{
	bTimerActivated = false;

	AIngredient* PlacedIngredient = Cast<AIngredient>(CookingPtr);
	PlacedIngredient->ChangeState(EIngredientState::EIS_CHOPPED);
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Turquoise, "Chopping Done");
	CookingPtr = Cast<ACooking>(PlacedIngredient);

	ProgressBar->bDestroy = true;
	ProgressBar = nullptr;

	ChefPtr->Chopping(false);
	ChefPtr = nullptr;
	bChoppingDone = false;
}

void AChoppingTable::TimerUpdate(float DeltaTime)
{
	Timer -= DeltaTime;
	CurTime = Timer;
}


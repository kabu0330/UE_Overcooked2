// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/ChoppingTable.h"
#include <Character/OC2Character.h>
#include <Global/OC2Enum.h>

AChoppingTable::AChoppingTable()
{

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
	}
}

ACooking* AChoppingTable::Interact(AActor* ChefActor)
{
	AOC2Character* Chef = Cast<AOC2Character>(ChefActor);
	ACooking* TempCooking = nullptr;

	if (CookingPtr != nullptr && false == Chef->IsHolding()) // À½½Ä ÀÖÀ½, ¼ÎÇÁ ºó ¼Õ
	{
		//ChopIngredient(ChefActor);
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
	if (nullptr != ChefActor)
	{
		if (true == CookingPtr->IsCookingType(ECookingType::ECT_INGREDIENT))
		{
			AIngredient* PlacedIngredient = Cast<AIngredient>(CookingPtr);

			if (true == PlacedIngredient->IsChoppable())
			{
				Timer = 3.0f;

				while (Timer > 0)
				{
					bTimerActivated = true;
				}

				bTimerActivated = false;
				PlacedIngredient->ChangeState(EIngredientState::EIS_CHOPPED);
				CookingPtr = Cast<ACooking>(PlacedIngredient);
			}
		}
	}
}

void AChoppingTable::TimerUpdate(float DeltaTime)
{
	Timer -= DeltaTime;
	CurTime = Timer;
}


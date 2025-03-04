// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Dev/CookingDevUserWidget.h"
#include <LevelContent/Cook/Ingredient.h>

void UCookingDevUserWidget::ChangeState(EIngredientState State)
{
	if (true == CookingActor.IsEmpty())
	{
		return;
	}

	for (int i = 0; i < CookingActor.Num(); i++)
	{
		AIngredient* Ingredient = Cast<AIngredient>(CookingActor[i]);
		if (State == Ingredient->GetCurIngredientState())
		{
			return;
		}
		else
		{
			Ingredient->ChangeState(State);
		}
	}
}

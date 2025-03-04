// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Dev/CookingDevUserWidget.h"
#include <LevelContent/Cook/Ingredient.h>

void UCookingDevUserWidget::ChangeState(EIngredientState State)
{
	AIngredient* Ingredient = Cast<AIngredient>(CookingActor);
	if (nullptr == Ingredient)
	{
		return;
	}

	if (State == Ingredient->GetCurIngredientState())
	{
		return;
	}
	else
	{
		Ingredient->ChangeState(State);
	}

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/Test/GlobalTestUserWidget.h"
#include "Overcooked2.h"

void UGlobalTestUserWidget::CreateIngredientActor()
{
	UOC2Global::SpawnIngredientActor(GetWorld(), EIngredientType::EIT_FISH);
}

void UGlobalTestUserWidget::CreatePlateActor()
{
	UOC2Global::SpawnPlateActor(GetWorld());
}

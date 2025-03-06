// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/NonTable/GarbageCan.h"

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
}

void AGarbageCan::PlaceItem(ACooking* Item)
{

}

void AGarbageCan::DestroyIngredient(AIngredient* WrongIngredient)
{
	Garbage = WrongIngredient;
	/*FQuat Rotation = 
	Garbage->AddActorWorldRotation()*/

	// ...쓰레기 회전시키기...
	Garbage->Destroy();
}
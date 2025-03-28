// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/Manager/TitleManager.h"

#include "Character/OC2Character.h"

void ATitleManager::BeginPlay()
{
	Super::BeginPlay();
}

void ATitleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATitleManager::InitCharacter()
{
	if (nullptr != OC2CharacterClass)
	{
		AOC2Character* OC2Character = GetWorld()->SpawnActor<AOC2Character>(OC2CharacterClass, FVector::ZeroVector, FRotator::ZeroRotator);
		OC2Character->SetActorLocation(UOC2Const::TitleCharacterSpawnLocation);
		OC2Character->SetActorRotation(UOC2Const::TitleCharacterSpawnRotation);
		OC2Character->SetCharacterName(UOC2Const::ChefEagleHeadName);
		OC2Character->Chopping(true);
	}
}

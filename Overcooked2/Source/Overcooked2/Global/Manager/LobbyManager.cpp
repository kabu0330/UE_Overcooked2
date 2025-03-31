// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/Manager/LobbyManager.h"

#include "Character/OC2Character.h"
#include "Global/State/GameState/LobbyGameState.h"
#include "Global/OC2GameInstance.h"
#include "Global/OC2Global.h"

ALobbyManager::ALobbyManager()
{

}

ALobbyManager* ALobbyManager::Get(UWorld* WorldContext)
{
	if (ALobbyGameState* LobbyGameState = WorldContext->GetGameState<ALobbyGameState>())
	{
		return LobbyGameState->LobbyManager;
	}

	return nullptr;
}

void ALobbyManager::BeginPlay()
{
	Super::BeginPlay();

	InitCharacter();
}

void ALobbyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALobbyManager::InitCharacter()
{
	if (nullptr != OC2CharacterClass)
	{
		AOC2Character* OC2Character = GetWorld()->SpawnActor<AOC2Character>(OC2CharacterClass, FVector::ZeroVector, FRotator::ZeroRotator);
		//OC2Character->DispatchBeginPlay();
		OC2Character->SetActorLocation(UOC2Const::TitleCharacterSpawnLocation);
		OC2Character->SetActorRotation(UOC2Const::TitleCharacterSpawnRotation);
		OC2Character->SetCharacterName(UOC2Global::GetOC2GameInstance(GetWorld())->GetChefHeadName());
		OC2Character->Chopping(true);
	}
}

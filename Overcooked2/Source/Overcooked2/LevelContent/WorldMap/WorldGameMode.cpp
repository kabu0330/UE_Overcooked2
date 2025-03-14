// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/WorldMap/WorldGameMode.h"
#include "LevelContent/WorldMap/WorldManager.h"
#include "LevelContent/WorldMap/WorldPlayer.h"
#include "Kismet/GameplayStatics.h"

AWorldGameMode::AWorldGameMode()
{
}

void AWorldGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), TEXT("WorldManager"), Actors);

	if (Actors.Num() > 0)
	{
		WorldManager = Cast<AWorldManager>(Actors[0]);
	}
}

void AWorldGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	/*static bool test = true;

	if (test)
	{
		test = false;
		HideAllClientCharacters();
	}*/
}

void AWorldGameMode::HideAllClientCharacters()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC)
		{
			APawn* PlayerPawn = PC->GetPawn();
			if (PlayerPawn == nullptr)
			{
				continue;
			}

			if (!PlayerPawn->HasAuthority())
			{
				PlayerPawn->SetActorHiddenInGame(true);
				PlayerPawn->SetActorEnableCollision(false);
			}
		}
	}
}
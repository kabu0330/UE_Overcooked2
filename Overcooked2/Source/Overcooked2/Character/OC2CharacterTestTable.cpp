// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OC2CharacterTestTable.h"

// Sets default values
AOC2CharacterTestTable::AOC2CharacterTestTable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("MyStaticMesh");
	StaticMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AOC2CharacterTestTable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOC2CharacterTestTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOC2CharacterTestTable::Highlight()
{
	StaticMesh->SetMaterial(0, HighlightMat);

}

void AOC2CharacterTestTable::OffHighlight()
{
	StaticMesh->SetMaterial(0, OriginMat);

}


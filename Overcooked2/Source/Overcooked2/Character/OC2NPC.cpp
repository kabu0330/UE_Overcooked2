// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OC2NPC.h"

// Sets default values
AOC2NPC::AOC2NPC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	RootComponent = MeshComponent;
}

// Called when the game starts or when spawned
void AOC2NPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOC2NPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


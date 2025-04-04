// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Title/TitleClouds.h"
#include "PaperSpriteComponent.h" 
#include "Kismet/KismetMathLibrary.h" 

// Sets default values
ATitleClouds::ATitleClouds()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	PaperSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSpriteComponent"));
	PaperSpriteComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ATitleClouds::BeginPlay()
{
	Super::BeginPlay();
	
	if (0 != PaperSprites.Num())
	{
		const int32 Index = UKismetMathLibrary::RandomIntegerInRange(0, PaperSprites.Num() - 1);

		PaperSpriteComponent->SetSprite(PaperSprites[Index]);
	}

}

// Called every frame
void ATitleClouds::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (-6000.0f >GetActorLocation().Y)
	{
		FVector InitPos(GetActorLocation().X, 10000.0f, GetActorLocation().Z);
		SetActorLocation(InitPos);
	}
}


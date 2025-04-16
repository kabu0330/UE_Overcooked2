// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Loading/TimeEventActor.h"

// Sets default values
ATimeEventActor::ATimeEventActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TimeEventComponent = CreateDefaultSubobject<UTimeEventComponent>(TEXT("TimeEventComponent"));


}

// Called when the game starts or when spawned
void ATimeEventActor::BeginPlay()
{
	Super::BeginPlay();

	//if (nullptr != SubclassWidget)
	//{
	//	LoadingWidget = CreateWidget<ULoadingWidget>(GetWorld(), SubclassWidget);
	//	if (nullptr != LoadingWidget)
	//	{
	//		//LoadingWidget->AddToViewport();  // ÇÊ¿ä ½Ã
	//	}
	//}
	
}

// Called every frame 
void ATimeEventActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


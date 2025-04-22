// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/LobbyMenuActor.h"
#include "UI/Lobby/LobbyMenuWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/WidgetInteractionComponent.h"

// Sets default values
ALobbyMenuActor::ALobbyMenuActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteraction"));
	WidgetInteraction->InteractionSource = EWidgetInteractionSource::Mouse;
	RootComponent = WidgetInteraction;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(WidgetInteraction);
	WidgetComponent->SetWidgetSpace(EWidgetSpace::World); // or Screen
	WidgetComponent->SetDrawSize(FVector2D(2000.f, 1500.f));

	if (LobbyMenuWidgetClass != nullptr)
	{
		WidgetComponent->SetWidgetClass(LobbyMenuWidgetClass);
	}
	WidgetComponent->SetRelativeScale3D({ 0.35f, 0.35f, 0.35f});


}

// Called when the game starts or when spawned
void ALobbyMenuActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALobbyMenuActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALobbyMenuActor::RemoveHoverEvent()
{
	//if (nullptr != WidgetComponent)
	//{
	//	UUserWidget* Widget = WidgetComponent->GetUserWidgetObject();
	//	if (Widget)
	//	{
	//		LobbyMenuWidget = Cast<ULobbyMenuWidget>(Widget);
	//		if (LobbyMenuWidget)
	//		{
	//			LobbyMenuWidget->RemoveHoverButton();
	//		}
	//	}
	//}

	WidgetInteraction->bEnableHitTesting = false;

}
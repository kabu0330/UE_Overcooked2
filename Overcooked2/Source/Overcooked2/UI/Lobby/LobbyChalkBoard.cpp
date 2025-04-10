// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/LobbyChalkBoard.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "UI/Lobby/LobbyChalkBoard.h"
#include "Lobby/LobbyUserWidget.h"

ALobbyChalkBoard::ALobbyChalkBoard(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// StaticMesh 설정
	UStaticMeshComponent* MeshComp = GetStaticMeshComponent();

	UStaticMesh* ChalkBoardMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Resources/UI/Title/Van/m_chalkboard_01.m_chalkboard_01"));
	if (ChalkBoardMesh)
	{
		MeshComp->SetStaticMesh(ChalkBoardMesh);
	}

	WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteraction"));
	WidgetInteraction->SetupAttachment(MeshComp);
	WidgetInteraction->InteractionSource = EWidgetInteractionSource::Mouse;
	// WidgetComponent 생성
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(WidgetInteraction);
	WidgetComponent->SetWidgetSpace(EWidgetSpace::World); // or Screen
	WidgetComponent->SetDrawSize(FVector2D(500.f, 300.f));

	if (ChalkBoardWidgetClass != nullptr)
	{
		WidgetComponent->SetWidgetClass(ChalkBoardWidgetClass);
	}

	WidgetComponent->SetRelativeLocation(FVector(-0.108249f, 0.12f, 0.65f));
	WidgetComponent->SetRelativeRotation(FRotator(0.9f, -69.7f, 0.0f));
	WidgetComponent->SetRelativeScale3D({0.001f, 0.001f, 0.001f});

	WidgetComponent->SetDrawSize({1280.f,1000.f});

	SetActorScale3D(FVector(150.f, 150.f, 150.f));
}

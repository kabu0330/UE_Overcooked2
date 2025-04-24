// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/ChoppingTable.h"
#include <Character/OC2Character.h>
#include <Global/OC2Enum.h>
#include "Global/GameMode/OC2GameMode.h"
#include "Components/WidgetComponent.h"
#include <LevelContent/Cook/Widget/GaugeTextureWidget.h>
#include <Net/UnrealNetwork.h>
#include <Global/Data/OC2GlobalData.h>
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

AChoppingTable::AChoppingTable()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	ProgressBarComponent = CreateDefaultSubobject<UWidgetComponent>("ProgressBar");
	ProgressBarComponent->SetupAttachment(RootComponent);

	KnifeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Knife");
	KnifeMeshComponent->SetupAttachment(RootComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("SoundFx");
	AudioComponent->SetupAttachment(RootComponent);

	NiagaraComponent0 = CreateDefaultSubobject<UNiagaraComponent>("Niagara0");
	NiagaraComponent0->SetupAttachment(RootComponent);
	NiagaraComponent0->SetAutoActivate(false);

	NiagaraComponent1 = CreateDefaultSubobject<UNiagaraComponent>("Niagara1");
	NiagaraComponent1->SetupAttachment(RootComponent);
	NiagaraComponent1->SetAutoActivate(false);

}

void AChoppingTable::BeginPlay()
{
	Super::BeginPlay();

	// 위젯 클래스 지정
	InitProgressBar();

	SoundEffect = UOC2GlobalData::GetTableBaseSound(GetWorld(), "KnifeChop");
	AudioComponent->SetSound(SoundEffect);

	InitNiagara();
}

void AChoppingTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckChefIsChopping();

	if (true == bTimerActivated)
	{
		if (true == bChopping)
		{
			TimerUpdate(DeltaTime);

			if (Timer > 2.0f)
			{
				bChoppingDone = true;
			}
		}
	}

	Ratio = (Timer / 0.4f) * 0.2f;
	WidgetPtr->SetProgressTimeRatio(Ratio);

	if (HasAuthority())
	{
		if (bChoppingDone == true)
		{
			ChoppingIsDone();
		}
	}

	HideKnife();
}

ACooking* AChoppingTable::Interact(AActor* ChefActor)
{
	ChefPtr = Cast<AOC2Character>(ChefActor);
	ACooking* TempCooking = nullptr;

	if (false == bChopping)
	{
		if (CookingPtr != nullptr && false == ChefPtr->IsHolding()) // 음식 있음, 셰프 빈 손
		{
			TempCooking = CookingPtr;
			CookingPtr = nullptr;
			return TempCooking;
		}
		else
		{
			return CookingPtr;
		}
	}
	else
	{
		return nullptr;
	}
}

void AChoppingTable::HideKnife_Implementation()
{
	if (nullptr != CookingPtr)
	{
		bCheckHidden = true;
	}
	else
	{
		bCheckHidden = false;
	}

	if (true == bCheckHidden)
	{
		KnifeMeshComponent->SetHiddenInGame(true);
	}
	else
	{
		KnifeMeshComponent->SetHiddenInGame(false);
	}
}

void AChoppingTable::SetNiagaraActive()
{
	NiagaraComponent0->SetActive(bNiagaraActive);
	NiagaraComponent1->SetActive(bNiagaraActive);
}

void AChoppingTable::OnRep_SetNiagaraActive()
{
	NiagaraComponent0->SetActive(bNiagaraActive);
	NiagaraComponent1->SetActive(bNiagaraActive);
}

void AChoppingTable::SetSoundFxActive()
{
	if (nullptr != AudioComponent && nullptr != SoundEffect)
	{
		if (true == bSoundFxActive)
		{
			AudioComponent->Play();
		}
		else
		{
			AudioComponent->Stop();
		}
	}
}

void AChoppingTable::OnRep_SetSoundFxActive()
{
	if (nullptr != AudioComponent && nullptr != SoundEffect)
	{
		if (true == bSoundFxActive)
		{
			AudioComponent->Play();
		}
		else
		{
			AudioComponent->Stop();
		}
	}
}


void AChoppingTable::ChopIngredient(AActor* ChefActor)
{
	ChefPtr = Cast<AOC2Character>(ChefActor);

	if (nullptr != ChefActor && nullptr != CookingPtr)
	{
		if (true == CookingPtr->IsCookingType(ECookingType::ECT_INGREDIENT))
		{
			AIngredient* PlacedIngredient = Cast<AIngredient>(CookingPtr);

			if (true == PlacedIngredient->IsChoppable())
			{
				ChefPtr->Chopping(true);

				if (false == bTimerActivated)
				{
					Timer = 0.0f;
				}

				bNiagaraActive = true;
				SetNiagaraActive();

				bSoundFxActive = true;
				SetSoundFxActive();

				bTimerActivated = true;
				bChopping = true;
				HideProgressBar(false);
			}
		}
	}
}

void AChoppingTable::HideProgressBar_Implementation(bool Value)
{
	ProgressBarComponent->SetHiddenInGame(Value);
}

void AChoppingTable::ChoppingIsDone_Implementation()
{
	if (false == bChoppingDone || false == bChopping)
	{
		return;
	}

	bTimerActivated = false;
	bChopping = false;
	bNiagaraActive = false;
	SetNiagaraActive();

	bSoundFxActive = false;
	SetSoundFxActive();

	AIngredient* PlacedIngredient = Cast<AIngredient>(CookingPtr);
	PlacedIngredient->ChangeState(EIngredientState::EIS_CHOPPED);

	CookingPtr = Cast<ACooking>(PlacedIngredient);

	if (EIngredientType::EIT_CUCUMBER == PlacedIngredient->GetIngredientType())
	{
		CookingPtr->AddActorWorldRotation(FRotator(0.0f, 90.0f, 0.0f));
	}

	if (EIngredientType::EIT_PRAWN == PlacedIngredient->GetIngredientType())
	{
		CookingPtr->AddActorWorldRotation(FRotator(0.0f, 90.0f, 0.0f));
		CookingPtr->AddActorWorldOffset(FVector::UnitX() * 40.0f + FVector::UnitY() * 40.0f);
	}

	HideProgressBar(true);

	ChefPtr->Chopping(false);
	ChefPtr = nullptr;
	bChoppingDone = false;
}

void AChoppingTable::TimerUpdate(float DeltaTime)
{
	Timer += DeltaTime;
	CurTime = Timer;
}

void AChoppingTable::CheckChefIsChopping()
{
	if (nullptr != ChefPtr)
	{
		if (false == ChefPtr->IsCooking())
		{
			bChopping = false;
			ChefPtr = nullptr;
			//StopSoundEffect();

			bSoundFxActive = false;
			SetSoundFxActive();
			
			bNiagaraActive = false;
			SetNiagaraActive();
		}
	}
}

//void AChoppingTable::PlaySoundEffect_Implementation()
//{
//	if (nullptr != AudioComponent && nullptr != SoundEffect)
//	{
//		AudioComponent->Play();
//	}
//}
//
//void AChoppingTable::StopSoundEffect_Implementation()
//{
//	if (nullptr != AudioComponent && nullptr != SoundEffect)
//	{
//		AudioComponent->Stop();
//	}
//}


void AChoppingTable::InitNiagara()
{
	if (nullptr != NiagaraComponent0)
	{
		FString DataTableRow = TEXT("ChoppingFx0");
		FResourceNiagaraDataRow NiagaraDataRow = UOC2GlobalData::GetResourceNiagaraDataRow(GetWorld(), *DataTableRow);
		if (nullptr != NiagaraDataRow.NiagaraSystem)
		{
			UNiagaraSystem* FX = NiagaraDataRow.NiagaraSystem;
			NiagaraComponent0->SetAsset(FX);

			NiagaraComponent0->SetRelativeLocation(NiagaraDataRow.Location);
			NiagaraComponent0->SetRelativeRotation(NiagaraDataRow.Rotation);
		}
	}

	if (nullptr != NiagaraComponent1)
	{
		FString DataTableRow = TEXT("ChoppingFx1");
		FResourceNiagaraDataRow NiagaraDataRow = UOC2GlobalData::GetResourceNiagaraDataRow(GetWorld(), *DataTableRow);
		if (nullptr != NiagaraDataRow.NiagaraSystem)
		{
			UNiagaraSystem* FX = NiagaraDataRow.NiagaraSystem;
			NiagaraComponent1->SetAsset(FX);

			NiagaraComponent1->SetRelativeLocation(NiagaraDataRow.Location);
			NiagaraComponent1->SetRelativeRotation(NiagaraDataRow.Rotation);
		}
	}
}


void AChoppingTable::InitProgressBar()
{
	ProgressBarComponent->SetWidgetClass(TSubClassWidget); // WBP 위젯으로 설정
	UUserWidget* UserWidget = ProgressBarComponent->GetUserWidgetObject();
	if (nullptr != UserWidget)
	{
		WidgetPtr = Cast<UGaugeTextureWidget>(UserWidget);
		WidgetPtr->SetPosition(FVector2D{ 5.0f, 10.0f });
	}
	ProgressBarComponent->SetDrawAtDesiredSize(true);   // 위젯의 실제 크기로 렌더
	ProgressBarComponent->SetPivot(FVector2D(0.5f, 0.5f)); // 중심 정렬
	ProgressBarComponent->SetWidgetSpace(EWidgetSpace::Screen); // 월드 공간에서 3D처럼 보이게
	ProgressBarComponent->bHiddenInGame = true;

	// 카메라를 향하도록 설정
	ProgressBarComponent->SetTwoSided(true);
	ProgressBarComponent->SetTickWhenOffscreen(true);
	ProgressBarComponent->SetWorldLocation(this->GetActorLocation());
}

void AChoppingTable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AChoppingTable, bCheckHidden);
	DOREPLIFETIME(AChoppingTable, bNiagaraActive);
	DOREPLIFETIME(AChoppingTable, bSoundFxActive);
	DOREPLIFETIME(AChoppingTable, ProgressBarComponent);
	DOREPLIFETIME(AChoppingTable, KnifeMeshComponent);
	DOREPLIFETIME(AChoppingTable, Ratio);
	DOREPLIFETIME(AChoppingTable, Timer);
	DOREPLIFETIME(AChoppingTable, bTimerActivated);
	DOREPLIFETIME(AChoppingTable, bChopping);
	DOREPLIFETIME(AChoppingTable, bChoppingDone);
	DOREPLIFETIME(AChoppingTable, bChopping);
	DOREPLIFETIME(AChoppingTable, ChefPtr);
	DOREPLIFETIME(AChoppingTable, AudioComponent);
}

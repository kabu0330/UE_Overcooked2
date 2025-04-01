// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/TableProgressBar/ProgressBarWidget.h"
#include "LevelContent/Table/TableProgressBar/ProgressBarComponent.h"

void UProgressBarWidget::BindProgress(class UProgressBarComponent* ProgressBarComponent)
{
	ProgressBarCompo = ProgressBarComponent;
	ProgressBarComponent->ProgressDelegate.AddUObject(this, &UProgressBarWidget::UpdateWidget);

}

void UProgressBarWidget::UpdateWidget()
{
	if (nullptr != ProgressBarCompo)
	{
		Ratio = ProgressBarCompo->GetRatio();
	}
}



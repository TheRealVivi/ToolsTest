// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetInsightWidget.h"
#include "Components/TextBlock.h"

void UAssetInsightWidget::NativeConstruct() 
{
	Super::NativeConstruct();

	if (FileSizeText) 
	{
		FileSizeText->SetText(FText::FromString(TEXT("File Size: N/A")));
	}
}

void UAssetInsightWidget::UpdateInsights(FText InText) 
{
	// Update member TextBlock widget text with selected asset insights
	if (FileSizeText)
	{
		FileSizeText->SetText(InText);
	}
}
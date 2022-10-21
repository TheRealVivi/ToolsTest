// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetInsightWidget.h"
#include "Components/TextBlock.h"

void UAssetInsightWidget::NativeConstruct() 
{
	Super::NativeConstruct();

	if (AssetNameText) 
	{
		AssetNameText->SetText(FText::FromString(TEXT("<no file selected>")));
	}

	if (FileSizeText) 
	{
		FileSizeText->SetText(FText::FromString(TEXT("File Size: N/A")));
	}
}

void UAssetInsightWidget::UpdateInsights(TArray<FString> InUpdates) 
{
	// Update member TextBlock widget text with selected asset insights
	if (AssetNameText) 
	{
		AssetNameText->SetText(FText::FromString("Asset Name: " + InUpdates[0]));
	}

	if (FileSizeText)
	{
		FileSizeText->SetText(FText::FromString("File Size: " + InUpdates[1] ));
	}
}
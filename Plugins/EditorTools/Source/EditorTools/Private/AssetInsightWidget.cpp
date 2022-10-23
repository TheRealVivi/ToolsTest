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

	if (NumOfActorsText) 
	{
		NumOfActorsText->SetText(FText::FromString(TEXT("# Actors: N/A")));
	}

	if (NumOfBlueprintsText)
	{
		NumOfBlueprintsText->SetText(FText::FromString(TEXT("# Blueprints: N/A")));
	}

	if (MapSizeText) 
	{
		MapSizeText->SetText(FText::FromString(TEXT("Cartesean Size: N/A")));
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
		FileSizeText->SetText(FText::FromString("File Size: " + InUpdates[1]));
	}

	if (NumOfActorsText)
	{
		NumOfActorsText->SetText(FText::FromString("# Actors: " + InUpdates[2]));
	}

	if (NumOfBlueprintsText)
	{
		NumOfBlueprintsText->SetText(FText::FromString("# Blueprints: " + InUpdates[3]));
	}

	if (MapSizeText) 
	{
		MapSizeText->SetText(FText::FromString("Cartesean Size: " + InUpdates[4]));
	}
}
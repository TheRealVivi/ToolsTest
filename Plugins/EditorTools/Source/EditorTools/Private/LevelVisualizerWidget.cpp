// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelVisualizerWidget.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/CheckBox.h"
#include "AssetInsightWidget.h"


#define DEBUG_MSG(x, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT(x), __VA_ARGS__));}

void ULevelVisualizerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (AnalyzeButton) 
	{
		AnalyzeButton->OnClicked.AddDynamic(this, &ThisClass::UpdateInsightWidget);
	}	

	if (IncludeNonCollidingCheckBox) 
	{
		IncludeNonCollidingCheckBox->SetIsChecked(false);
	}
}

void ULevelVisualizerWidget::UpdateInsightWidget() 
{
	if (InsightWidget && IncludeNonCollidingCheckBox)
	{
		InsightWidget->UpdateInsights(IncludeNonCollidingCheckBox->IsChecked());
	}
}
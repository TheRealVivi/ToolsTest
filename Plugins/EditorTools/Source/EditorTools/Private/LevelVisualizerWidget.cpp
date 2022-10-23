// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelVisualizerWidget.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/TextBlock.h"
#include "AssetInsightWidget.h"


#define DEBUG_MSG(x, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT(x), __VA_ARGS__));}

void ULevelVisualizerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_0) 
	{
		Button_0->OnClicked.AddDynamic(this, &ThisClass::UpdateInsightWidget);
		Button_0->SetRenderOpacity(.2f);
	}

	if (SomeText) 
	{
		SomeText->SetText(FText::FromString(TEXT("Hello world! lolol")));
		SomeText->SetOpacity(.1f);
	}

	
}

void ULevelVisualizerWidget::UpdateInsightWidget() 
{
	if (InsightWidget) 
	{
		InsightWidget->UpdateInsights();
	}
}
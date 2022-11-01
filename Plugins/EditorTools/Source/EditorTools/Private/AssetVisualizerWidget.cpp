// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetVisualizerWidget.h"
#include "SAssetVisualizerWidget.h"

void UAssetVisualizerWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyAssetVisualizerWidget.Reset();
}

void UAssetVisualizerWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (MyAssetVisualizerWidget)
	{
		MyAssetVisualizerWidget->SetDisplayedText(Text);
	}
}

TSharedRef<SWidget> UAssetVisualizerWidget::RebuildWidget()
{
	MyAssetVisualizerWidget = SNew(SAssetVisualizerWidget)
		.DisplayedText(Text);

	return MyAssetVisualizerWidget.ToSharedRef();
}
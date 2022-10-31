// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetVisualizerWidget.h"
#include "SAssetVisualizerWidget.h"

void UAssetVisualizerWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	UE_LOG(LogTemp, Warning, TEXT("ReleaseSlateResources! %d"), this->GetUniqueID());
	MyAssetVisualizerWidget.Reset();
}

void UAssetVisualizerWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	UE_LOG(LogTemp, Warning, TEXT("SynchronizeProperties!"));
	if (MyAssetVisualizerWidget)
	{
		MyAssetVisualizerWidget->SetDisplayedText(Text);
	}
}

TSharedRef<SWidget> UAssetVisualizerWidget::RebuildWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("RebuildWidget!"));
	MyAssetVisualizerWidget = SNew(SAssetVisualizerWidget)
		.DisplayedText(Text);

	return MyAssetVisualizerWidget.ToSharedRef();
}
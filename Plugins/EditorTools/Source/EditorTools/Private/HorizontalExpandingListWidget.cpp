// Fill out your copyright notice in the Description page of Project Settings.


#include "HorizontalExpandingListWidget.h"
#include "SHorizontalExpandingListWidget.h"

void UHorizontalExpandingListWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	UE_LOG(LogTemp, Warning, TEXT("ReleaseSlateResources called!"));
	Super::ReleaseSlateResources(bReleaseChildren);
	MyHorizontalExpandingListWidget.Reset();
}

void UHorizontalExpandingListWidget::SynchronizeProperties()
{
	UE_LOG(LogTemp, Warning, TEXT("SyncProp called!"));
	Super::SynchronizeProperties();
	if (MyHorizontalExpandingListWidget)
	{
		MyHorizontalExpandingListWidget->SetTextForTextBlocks(TextForTextBlocks);
		MyHorizontalExpandingListWidget->SetTextBlockCount(TextBlockCount);
		MyHorizontalExpandingListWidget->SetVerticalBoxCount(VerticalBoxCount);
	}

}

void UHorizontalExpandingListWidget::SetTextForTextBlocks(TArray<FString> InTextForTextBlocks)
{
	this->TextForTextBlocks = InTextForTextBlocks;

	UE_LOG(LogTemp, Warning, TEXT("SetTextForTextBlocks! TextForTextBlock size: %d"), TextForTextBlocks.Num());

	if (MyHorizontalExpandingListWidget) 
	{
		this->TextBlockCount = InTextForTextBlocks.Num();
		this->VerticalBoxCount = (InTextForTextBlocks.Num() / 5) + 1;
		MyHorizontalExpandingListWidget->SetTextForTextBlocks(InTextForTextBlocks);
		MyHorizontalExpandingListWidget->SetTextBlockCount(this->TextBlockCount);
		MyHorizontalExpandingListWidget->SetVerticalBoxCount(this->VerticalBoxCount);
	}
}

TSharedRef<SWidget> UHorizontalExpandingListWidget::RebuildWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("Rebuild called!"));
	MyHorizontalExpandingListWidget = SNew(SHorizontalExpandingListWidget)
		.TextForTextBlocks(TextForTextBlocks)
		.TextBlockCount(TextBlockCount)
		.VerticalBoxCount(VerticalBoxCount);

	return MyHorizontalExpandingListWidget.ToSharedRef();
}

void UHorizontalExpandingListWidget::UpdateWidget() 
{
	RebuildWidget();
}
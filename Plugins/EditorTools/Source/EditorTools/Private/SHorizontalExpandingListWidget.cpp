// Fill out your copyright notice in the Description page of Project Settings.


#include "SHorizontalExpandingListWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SHorizontalExpandingListWidget::Construct(const FArguments& InArgs)
{
	UE_LOG(LogTemp, Warning, TEXT("Slate Construct!"));
	TextBlockCount = InArgs._TextBlockCount;
	VerticalBoxCount = InArgs._VerticalBoxCount;
	TextForTextBlocks = InArgs._TextForTextBlocks;

	ChildSlot
	[
		SAssignNew(ListBox, SHorizontalBox)
	];
	
	RebuildFromData();
}

void SHorizontalExpandingListWidget::RebuildFromData() 
{
	//UE_LOG(LogTemp, Warning, TEXT("SlateRebuildFromData! TextForTextBlock size: %d"), TextForTextBlocks.Num());
	

	ListBox->ClearChildren();
	//VertListBoxes.Empty();
	//VertListBoxes.Reserve(VerticalBoxCount);
	VertListBoxes.Empty();
	//VertListBoxes.Add(TSharedPtr<SVerticalBox>());
	//TextForTextBlocks.Empty();
	//TextForTextBlocks.Add("Hello");
	//TextForTextBlocks.Add("World!");
	UE_LOG(LogTemp, Warning, TEXT("VerticalBoxCount: %d TextBlockCount: %d"), VerticalBoxCount, TextBlockCount);
	//VerticalBoxCount = 1;
	//TextBlockCount = TextForTextBlocks.Num();

	//VerticalBoxCount = (TextBlockCount / 5) + 1;

	for (int32 i = 0; i < VerticalBoxCount; i++)
	{
		VertListBoxes.Add(TSharedPtr<SVerticalBox>());
	}

	for (int32 i = 0; i < VertListBoxes.Num(); ++i)
	{
		ListBox->AddSlot()
		[
			SAssignNew(VertListBoxes[i], SVerticalBox)
		];
	}

	for (int32 i = 0; i < TextBlockCount; ++i) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("Inside slate: %s, VertListBoxes size: %d"), *TextForTextBlocks[i], VertListBoxes.Num());
		VertListBoxes[i / 5]->AddSlot()
		[
			SNew(STextBlock)
			//.Text(FText::FromString("Hello" + i))
			.Text(FText::FromString(TextForTextBlocks[i]))
		];
	}
}

void SHorizontalExpandingListWidget::SetTextBlockCount(int32 InTextBlockCount)
{
	this->TextBlockCount = InTextBlockCount;
}

void SHorizontalExpandingListWidget::SetVerticalBoxCount(int32 InVerticalBoxCount)
{
	this->VerticalBoxCount = InVerticalBoxCount;
}

void SHorizontalExpandingListWidget::SetTextForTextBlocks(TArray<FString> InTextForTextBlocks)
{
	this->TextForTextBlocks = InTextForTextBlocks;

	UE_LOG(LogTemp, Warning, TEXT("TBlockCount: %d, VBoxCount: %d, TextforTextBlock size: %d"), this->TextBlockCount, this->VerticalBoxCount, this->TextForTextBlocks.Num());
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION


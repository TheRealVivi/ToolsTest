// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class EDITORTOOLS_API SHorizontalExpandingListWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHorizontalExpandingListWidget)
		{}
		SLATE_ARGUMENT(int32, TextBlockCount)
		SLATE_ARGUMENT(int32, VerticalBoxCount)
		SLATE_ARGUMENT(TArray<FString>, TextForTextBlocks)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	void RebuildFromData();
	void SetTextBlockCount(int32 InTextBlockCount);
	void SetVerticalBoxCount(int32 InVerticalBoxCount);
	void SetTextForTextBlocks(TArray<FString> InTextForTextBlocks);

protected:
	TArray<FString> TextForTextBlocks;
	TSharedPtr<SHorizontalBox> ListBox;
	TArray<TSharedPtr<SVerticalBox>> VertListBoxes;
	int32 TextBlockCount = 0;
	int32 VerticalBoxCount = 0;
};

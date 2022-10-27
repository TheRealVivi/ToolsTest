// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SAssetVisualizerWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAssetVisualizerWidget)
		{}
		SLATE_ARGUMENT(FText, DisplayedText)
	SLATE_END_ARGS()

	/**
	 * Construct the widget
	 *
	 * @param	InArgs				A declaration from which to construct the widget
	 */
	void Construct(const FArguments& InArgs);

	void SetDisplayedText(FText DisplayedText);

protected:
	FText Text;
	TSharedPtr<STextBlock> TextBlock;
};

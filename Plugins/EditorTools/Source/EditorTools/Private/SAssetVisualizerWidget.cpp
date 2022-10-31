// Fill out your copyright notice in the Description page of Project Settings.


#include "SAssetVisualizerWidget.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SConstraintCanvas.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SAssetVisualizerWidget::Construct(const FArguments& InArgs)
{
	Text = InArgs._DisplayedText;

	ChildSlot
	[
		/*
		SNew(SConstraintCanvas)
		+SConstraintCanvas::Slot()
		[
			//SNew(SImage)
			//.Image()
			//SAssignNew(TextBlock, STextBlock)
			//.Text(InArgs._DisplayedText)
		]
		*/
		
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(TextBlock, STextBlock)
			.Text(InArgs._DisplayedText)
		]
		
	];
}

void SAssetVisualizerWidget::SetDisplayedText(FText NewText) 
{
	this->Text = NewText;
	this->TextBlock->SetText(NewText);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION



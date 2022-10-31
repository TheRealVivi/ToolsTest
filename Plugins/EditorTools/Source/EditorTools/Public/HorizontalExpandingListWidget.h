// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "HorizontalExpandingListWidget.generated.h"

/**
 * 
 */
UCLASS()
class EDITORTOOLS_API UHorizontalExpandingListWidget : public UWidget
{
	GENERATED_BODY()
	
public:
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void SynchronizeProperties() override;
	
	void SetTextForTextBlocks(TArray<FString> InTextForTextBlocks);
	void UpdateWidget();
	

protected:
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

	TArray<FString> TextForTextBlocks;

	UPROPERTY(EditAnywhere)
	int32 TextBlockCount = 0;

	UPROPERTY(EditAnywhere)
	int32 VerticalBoxCount = 0;


	TSharedPtr<class SHorizontalExpandingListWidget> MyHorizontalExpandingListWidget;
};

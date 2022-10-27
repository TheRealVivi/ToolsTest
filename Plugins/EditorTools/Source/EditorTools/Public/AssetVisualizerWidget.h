// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "AssetVisualizerWidget.generated.h"

/**
 * 
 */
UCLASS()
class EDITORTOOLS_API UAssetVisualizerWidget : public UWidget
{
	GENERATED_BODY()
	
public:
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void SynchronizeProperties() override;

protected:
	UPROPERTY(EditAnywhere)
	FText Text;

	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

	TSharedPtr<class SAssetVisualizerWidget> MyAssetVisualizerWidget;
};

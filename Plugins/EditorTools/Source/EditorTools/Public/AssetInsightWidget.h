// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AssetInsightWidget.generated.h"

/**
 * 
 */
UCLASS()
class EDITORTOOLS_API UAssetInsightWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UTextBlock* AssetNameText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* FileSizeText;

	void UpdateInsights(TArray<FString> InUpdates); // TODO: add parameters with array of strings to update TextBlocks with
};

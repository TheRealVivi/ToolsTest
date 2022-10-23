// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AssetInsightWidget.generated.h"

UENUM()
enum class ETextBlockTypes : uint8
{
	ASSET_NAME_TEXT = 0,
	FILE_SIZE_TEXT,
	NUM_ACTORS_TEXT,
	NUM_BLUEPRINTS_TEXT,
};

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

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* NumOfActorsText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* NumOfBlueprintsText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* MapSizeText;

	void UpdateInsights(TArray<FString> InUpdates); // TODO: add parameters with array of strings to update TextBlocks with
};

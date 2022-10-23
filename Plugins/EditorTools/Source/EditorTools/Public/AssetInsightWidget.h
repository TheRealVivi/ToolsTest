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
 * User widget that contains import metrics related to selected asset
 * NOTE: Currently only applicable to ULevel/UWorld assets
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

	/** TODO: Refactor in to specific encapsulated functions
	 *  TODO: Pull actor info from MapBuildData for levels that store data in MapBuildData classes */
	void UpdateInsights();

private:

	/** Updates widget text blocks with data from parameter array */
	void UpdateTextBlocks(TArray<FString> InUpdates);

	/** Copy/paste from FLevelBounds class. */
	FBox CalculateLevelBounds(const ULevel* InLevel);
};

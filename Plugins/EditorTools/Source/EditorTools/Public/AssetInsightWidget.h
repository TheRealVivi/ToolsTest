// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AssetInsightWidget.generated.h"

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

	/** TODO: Pull actor info from External packages to cover external package user case */
	void UpdateInsights();

protected:

	/** Updates widget text blocks with data from parameter array */
	void UpdateTextBlocks(TArray<FString> InUpdates);

	/** Copy/paste from FLevelBounds class. */
	FBox CalculateLevelBounds(const ULevel* InLevel);

	/** Checks if level's asset data has tag values for level bounds, if not, calculates them
	 *  Returns level bounds as a FVector3d
	*/
	FVector3d GetLevelBounds(const ULevel* InLevel, FAssetData* LevelAssetData);

	/** Checks each actor in level to see if it is attached to a blueprint; returns -1 if InLevel is null*/
	int32 GetNumBlueprintsInLevel(const ULevel* InLevel);
};

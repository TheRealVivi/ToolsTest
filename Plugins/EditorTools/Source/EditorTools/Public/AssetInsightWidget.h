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

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* ActorHierarchyDetails;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* ActorHierarchyRelationship;

	//UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	//class UHorizontalExpandingListWidget* ActorHierarchyDetails;

	/** TODO: Pull actor info from External packages to cover external package user case */
	void UpdateInsights();

protected:

	/** Updates widget text blocks with data from parameter array */
	void UpdateTextBlocks(TArray<FString> InUpdates, TArray<FString>* InHierarchyUpdates, TArray<FString>* InHierarchyRelationship);

	FString UpdateHierarchyDetails(TArray<FString>* InHierarchyUpdates);
	FString UpdateHierarchyRelationship(TArray<FString>* InHierarchyRelationship);

	/** Copy/paste from FLevelBounds class. */
	FBox CalculateLevelBounds(const ULevel* InLevel);

	TMap<FString, FBox> CalculateLevelHierarchyBounds(const ULevel* InLevel);
	TMap<FString, TArray<FString>> CalculateLevelHierarchyRelationship(const ULevel* InLevel);

	/** Checks if level's asset data has tag values for level bounds, if not, calculates them
	 *  Returns level bounds as a FVector3d
	*/
	FVector3d GetLevelBounds(const ULevel* InLevel, FAssetData* LevelAssetData);

	void GetActorHierarchyFootprint(const ULevel* InLevel, TArray<FString>* OutHierarchyFootprint);
	void GetActorHierarchyRelationship(const ULevel* InLevel, TArray<FString>* OutHierarchyRelationship);

	/** Checks each actor in level to see if it is attached to a blueprint; returns -1 if InLevel is null */
	int32 GetNumBlueprintsInLevel(const ULevel* InLevel);

private:
	const int32 ASSET_NAME = 0;
	const int32 FILE_SIZE = 1;
	const int32 ACTOR_TEXT = 2;
	const int32 BLUEPRINT_TEXT = 3;
	const int32 MAP_SIZE = 4;
};

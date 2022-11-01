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
	void UpdateInsights(bool bInIncludeNonColliding);

protected:

	/** Updates widget text blocks with data from parameter array */
	void UpdateTextBlocks(TArray<FString> InUpdates, TArray<FString>* InHierarchyUpdates, TArray<FString>* InHierarchyRelationship);

	/**
	 * \Brief:   Prepares string with desired formatting for ActorHierarchyDetails TextBlock
	 * \Returns: FString desired formatting for ActorHierarchyDetails TextBlock
	*/
	FString UpdateHierarchyDetails(TArray<FString>* InHierarchyUpdates);

	/**
	 * \Brief:   Prepares string with desired formatting for ActorHierarchyRelationship TextBlock
	 * \Returns: FString desired formatting for ActorHierarchyRelationship TextBlock
	*/
	FString UpdateHierarchyRelationship(TArray<FString>* InHierarchyRelationship);

	/** 
	 * \Note:    Copy/paste from FLevelBounds class. 
	 * \Brief:   Calculates level bounds
	 *           If bIncludeNonColliding is true, will add bounding boxes of non-colliding actors
	 * \Returns: An FBox with all of the level actors bounding boxes summed up
	*/
	FBox CalculateLevelBounds(const ULevel* InLevel);

	/**
	 * \Brief:   Uses a TMap to map out the Level's Actor hierarchy
	 * \Returns: TMap with Level's Actor hierarchy and summed up bounding boxes
	*/
	TMap<FString, FBox> CalculateLevelHierarchyBounds(const ULevel* InLevel);

	/**
	 * \Brief:   Uses a TMap to map out the Level's Actor hierarchy relationship
	 * \Returns: TMap with Level's Actor hierarchy relationships
	*/
	TMap<FString, TArray<FString>> CalculateLevelHierarchyRelationship(const ULevel* InLevel);

	/** 
	 * \Brief:   Calculates level bounds
	 * \Returns: FVector3d of level bounds size
	*/
	FVector3d GetLevelSize(const ULevel* InLevel);

	/**
	 * \Brief:   Uses a TMap to map out the Level's Actor hierarchy,
	 *           Given TArray reference will be filled with hierarchy and it's size
	*/
	void GetActorHierarchyFootprint(const ULevel* InLevel, TArray<FString>* OutHierarchyFootprint);

	/**
	 * \Brief:   Uses a TMap to map out the Level's Actor hierarchy
	 *           Given TArray reference will be filled with the hierarchy relationships
	*/
	void GetActorHierarchyRelationship(const ULevel* InLevel, TArray<FString>* OutHierarchyRelationship);

	/**
	 * \Brief:   Sums up number of blueprints referenced InLevel
	 * \Returns: Returns number of blueprints. Will return -1 if InLevel is null
	*/
	int32 GetNumBlueprintsInLevel(const ULevel* InLevel);

private:

	/** const int32 for clear indexing in UpdateTextBlocks */
	const int32 ASSET_NAME = 0;
	const int32 FILE_SIZE = 1;
	const int32 ACTOR_TEXT = 2;
	const int32 BLUEPRINT_TEXT = 3;
	const int32 MAP_SIZE = 4;

	/** Used by UpdateInsights to determine whether to calculate bounding boxes with non-colliding */
	bool bIncludeNonColliding;
};

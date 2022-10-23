// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetInsightWidget.h"
#include "Components/TextBlock.h"
#include "EditorUtilityLibrary.h"

void UAssetInsightWidget::NativeConstruct() 
{
	Super::NativeConstruct();

	if (AssetNameText) 
	{
		AssetNameText->SetText(FText::FromString(TEXT("<no file selected>")));
	}

	if (FileSizeText) 
	{
		FileSizeText->SetText(FText::FromString(TEXT("File Size: N/A")));
	}

	if (NumOfActorsText) 
	{
		NumOfActorsText->SetText(FText::FromString(TEXT("# Actors: N/A")));
	}

	if (NumOfBlueprintsText)
	{
		NumOfBlueprintsText->SetText(FText::FromString(TEXT("# Blueprints: N/A")));
	}

	if (MapSizeText) 
	{
		MapSizeText->SetText(FText::FromString(TEXT("Cartesean Size: N/A")));
	}
}

/** Updates widget text blocks with data from parameter array */
void UAssetInsightWidget::UpdateTextBlocks(TArray<FString> InUpdates) 
{
	// Update member TextBlock widget text with selected asset insights
	if (AssetNameText) 
	{
		AssetNameText->SetText(FText::FromString("Asset Name: " + InUpdates[0]));
	}

	if (FileSizeText)
	{
		FileSizeText->SetText(FText::FromString("File Size: " + InUpdates[1] + " bytes"));
	}

	if (NumOfActorsText)
	{
		NumOfActorsText->SetText(FText::FromString("# Actors: " + InUpdates[2]));
	}

	if (NumOfBlueprintsText)
	{
		NumOfBlueprintsText->SetText(FText::FromString("# Blueprints: " + InUpdates[3]));
	}

	if (MapSizeText) 
	{
		MapSizeText->SetText(FText::FromString("Cartesean Size: " + InUpdates[4]));
	}
}

/** Copy/paste from FLevelBounds class. */
FBox UAssetInsightWidget::CalculateLevelBounds(const ULevel* InLevel)
{
	FBox LevelBounds(ForceInit);

	if (InLevel)
	{
		// Iterate over all level actors
		for (int32 ActorIndex = 0; ActorIndex < InLevel->Actors.Num(); ++ActorIndex)
		{
			AActor* Actor = InLevel->Actors[ActorIndex];
			
			if (Actor && Actor->IsLevelBoundsRelevant())
			{
				// Sum up components bounding boxes
				FBox ActorBox = Actor->GetComponentsBoundingBox(true);
				if (ActorBox.IsValid)
				{
					LevelBounds += ActorBox;
				}
			}
		}
	}

	return LevelBounds;
}

/** 
 *  TODO: Pull actor info from External packages to cover external package user case
*/
void UAssetInsightWidget::UpdateInsights()
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateInsights() called!"));

	TArray<FAssetData> SelectedObjectsAssetData = UEditorUtilityLibrary::GetSelectedAssetData();
	
	if (SelectedObjectsAssetData.Num() != 1)
		return;

	TArray<FString> UpdateInfo;
	FAssetData SelectedAssetData = SelectedObjectsAssetData[0];
	UWorld* world = Cast<UWorld>(SelectedAssetData.GetAsset());

	if (world)
	{
		
		if(world->GetCurrentLevel()->IsUsingExternalActors()) 
		{
			UE_LOG(LogTemp, Warning, TEXT("Is using external actors"));
			UE_LOG(LogTemp, Warning, TEXT("Num of actors from external package: %d"), world->GetCurrentLevel()->Actors.Num());
			
		}			

		UpdateInfo.Add(*SelectedAssetData.AssetName.ToString());
		UpdateInfo.Add(FString::FromInt(SelectedAssetData.GetPackage()->GetFileSize()));
		UpdateInfo.Add(FString::FromInt(world->GetCurrentLevel()->Actors.Num()));
		UpdateInfo.Add(FString::FromInt(GetNumBlueprintsInLevel(world->GetCurrentLevel())));
		UpdateInfo.Add(GetLevelBounds(world->GetCurrentLevel(), &SelectedAssetData).ToString());
	}

	UpdateTextBlocks(UpdateInfo);
}

/** Checks each actor in level to see if it is attached to a blueprint; returns -1 if InLevel is null*/
int32 UAssetInsightWidget::GetNumBlueprintsInLevel(const ULevel* InLevel) 
{
	int32 numOfBlueprints = -1;

	if (InLevel) 
	{
		//UMapBuildDataRegistry* mapData = InLevel->MapBuildData.Get();

		numOfBlueprints = 0;
		TArray<AActor*> LevelActors = InLevel->Actors;

		for (AActor* a : LevelActors)
		{
			if (a->GetArchetype()->IsInBlueprint())
			{
				//UE_LOG(LogTemp, Warning, TEXT("part of blueprint: %s"), *a->GetFName().ToString());
				numOfBlueprints++;
			}
		}
	}

	return numOfBlueprints;
}

/** Checks if level's asset data has tag values for level bounds, if not, calculates them
 *  Returns level bounds as a FVector3d
*/
FVector3d UAssetInsightWidget::GetLevelBounds(const ULevel* InLevel, FAssetData* LevelAssetData)
{
	FVector3d levelSize;
	FBox levelBounds;

	FString LevelBoundsLocationStr;
	static const FName NAME_LevelBoundsLocation(TEXT("LevelBoundsLocation"));

	FString LevelBoundsExtentStr;
	static const FName NAME_LevelBoundsExtent(TEXT("LevelBoundsExtent"));

	if (LevelAssetData && InLevel) 
	{

		if (LevelAssetData->GetTagValue(NAME_LevelBoundsLocation, LevelBoundsLocationStr) &&
			LevelAssetData->GetTagValue(NAME_LevelBoundsExtent, LevelBoundsExtentStr))
		{
			ULevel::GetLevelBoundsFromAsset(*LevelAssetData, levelBounds);
		}
		else
		{
			levelBounds = CalculateLevelBounds(InLevel);
		}
	}

	levelSize = levelBounds.GetSize();

	return levelSize;
}
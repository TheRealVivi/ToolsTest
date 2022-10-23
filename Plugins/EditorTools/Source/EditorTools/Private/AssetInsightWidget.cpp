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
		FileSizeText->SetText(FText::FromString("File Size: " + InUpdates[1]));
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

/** TODO: Refactor in to specific encapsulated functions
 *  TODO: Pull actor info from MapBuildData for levels that store data in MapBuildData classes
*/
void UAssetInsightWidget::UpdateInsights()
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateInsights() called!"));

	TArray<FAssetData> SelectedObjectsAssetData = UEditorUtilityLibrary::GetSelectedAssetData();
	TArray<FString> UpdateInfo;
	FVector3d levelSize;


	if (SelectedObjectsAssetData.Num() != 1)
		return;


	FAssetData SelectedAssetData = SelectedObjectsAssetData[0];
	SelectedAssetData.PrintAssetData();
	UWorld* world = Cast<UWorld>(SelectedAssetData.GetAsset());
	int32 numOfActors, numOfBlueprints = 0;

	if (world)
	{
		FBox levelBounds;
		TArray<AActor*> LevelActors = world->GetCurrentLevel()->Actors;

		FString LevelBoundsLocationStr;
		static const FName NAME_LevelBoundsLocation(TEXT("LevelBoundsLocation"));

		FString LevelBoundsExtentStr;
		static const FName NAME_LevelBoundsExtent(TEXT("LevelBoundsExtent"));

		if (SelectedAssetData.GetTagValue(NAME_LevelBoundsLocation, LevelBoundsLocationStr) &&
			SelectedAssetData.GetTagValue(NAME_LevelBoundsExtent, LevelBoundsExtentStr))
		{
			ULevel::GetLevelBoundsFromAsset(SelectedAssetData, levelBounds);
		}
		else
		{
			levelBounds = CalculateLevelBounds(world->GetCurrentLevel());
		}

		levelSize = levelBounds.GetSize();

		//world->GetCurrentLevel()->MapBuildData;

		numOfActors = LevelActors.Num();
		//FVector worldCenter = world->GetModel()->GetCenter();
		for (AActor* a : LevelActors)
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor name: %s"), *a->GetFName().ToString());
			if (a->GetFName().ToString().StartsWith("BP_") || a->GetFName().ToString().StartsWith("Blueprint_"))
				numOfBlueprints++;
		}

		UE_LOG(LogTemp, Warning, TEXT("Level size: %s"), *levelSize.ToString());
	}

	UpdateInfo.Add(*SelectedAssetData.AssetName.ToString());
	UpdateInfo.Add(FString::FromInt(SelectedAssetData.GetPackage()->GetFileSize()).Append(" bytes"));
	UpdateInfo.Add(FString::FromInt(numOfActors));
	UpdateInfo.Add(FString::FromInt(numOfBlueprints));
	UpdateInfo.Add(levelSize.ToString());

	UpdateTextBlocks(UpdateInfo);
}
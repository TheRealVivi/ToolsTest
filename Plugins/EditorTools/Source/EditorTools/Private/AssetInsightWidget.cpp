// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetInsightWidget.h"
#include "Components/TextBlock.h"
#include "HorizontalExpandingListWidget.h"
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

	if (ActorHierarchyDetails) 
	{
		ActorHierarchyDetails->SetText(FText::FromString(TEXT("Actor Hierarchy Details: N/A")));
	}
	
	/*
	if (ActorHierarchyDetails) 
	{
		TArray<FString> DefaultData = { "Hierarchy size: N/A", "Select level asset to load" };
		ActorHierarchyDetails->SetTextForTextBlocks(DefaultData);
	}
	*/
	
}

/** Updates widget text blocks with data from parameter array */
void UAssetInsightWidget::UpdateTextBlocks(TArray<FString> InUpdates, TArray<FString> InHierarchyUpdates) 
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

	if (ActorHierarchyDetails) 
	{
		ActorHierarchyDetails->SetText(FText::FromString(UpdateHierarchyDetails(InHierarchyUpdates)));
	}
}

FString UAssetInsightWidget::UpdateHierarchyDetails(TArray<FString> InHierarchyUpdates) 
{
	FString HierarchyDetails = "";

	if (!InHierarchyUpdates.IsEmpty()) 
	{
		for (FString Detail : InHierarchyUpdates)
		{
			HierarchyDetails += Detail + "\n";
		}
	}
	else
	{
		HierarchyDetails = "No hierarchy details to display.";
	}

	return HierarchyDetails;
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

TMap<FString, FBox> UAssetInsightWidget::CalculateLevelHierarchyBounds(const ULevel* InLevel)
{
	TMap<FString, FBox> HierarchyBounds;
	// Calculate hierarchy bounds using folders
	if (InLevel->IsUsingActorFolders())
	{
		// use a TMap<Folder Name, Bounds> to calculate hierarchy bounds

		for (AActor* Actor : InLevel->Actors)
		{
			FString ActorFolderName = Actor->GetFolder().ToString();
			if (!HierarchyBounds.Contains(ActorFolderName))
			{
				HierarchyBounds.Add(ActorFolderName, Actor->GetComponentsBoundingBox());
				//UE_LOG(LogTemp, Warning, TEXT("Newly added Folder name: %s New bounds size: %s"), *ActorFolderName, *HierarchyBounds[ActorFolderName].GetSize().ToString());
			}
			else
			{
				HierarchyBounds[ActorFolderName] += Actor->GetComponentsBoundingBox();
				//UE_LOG(LogTemp, Warning, TEXT("Existing Folder name: %s New bounds size: %s"), *ActorFolderName, *HierarchyBounds[ActorFolderName].GetSize().ToString());
			}

			//UE_LOG(LogTemp, Warning, TEXT("Folder name: %s"), *Actor->GetFolder().ToString());
		}

	}
	return HierarchyBounds;
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
	TArray<FString> HierarchySizeDetails;
	FAssetData SelectedAssetData = SelectedObjectsAssetData[0];
	UWorld* World = Cast<UWorld>(SelectedAssetData.GetAsset());

	if (World)
	{
		ULevel* Level = World->GetCurrentLevel();

		TMap<FString, FBox> HierarchyBounds = CalculateLevelHierarchyBounds(Level);

		
		//HierarchySizeDetails.Reserve(HierarchyBounds.Num());

		for (TPair<FString, FBox> HierarchyBoundPair : HierarchyBounds) 
		{
			HierarchySizeDetails.Add(HierarchyBoundPair.Key + ": " + HierarchyBoundPair.Value.GetSize().ToString());
		}

		if(Level->IsUsingExternalActors()) 
		{
			UE_LOG(LogTemp, Warning, TEXT("Is using external actors"));
			UE_LOG(LogTemp, Warning, TEXT("Num of actors from external package: %d"), Level->Actors.Num());
			
		}			

		UpdateInfo.Add(*SelectedAssetData.AssetName.ToString());
		UpdateInfo.Add(FString::FromInt(SelectedAssetData.GetPackage()->GetFileSize()));
		UpdateInfo.Add(FString::FromInt(Level->Actors.Num()));
		UpdateInfo.Add(FString::FromInt(GetNumBlueprintsInLevel(Level)));
		UpdateInfo.Add(GetLevelBounds(Level, &SelectedAssetData).ToString());

		UE_LOG(LogTemp, Warning, TEXT("HierarchySizeDetails size: %d"), HierarchySizeDetails.Num());
		//ActorHierarchyDetails->SetTextForTextBlocks(HierarchySizeDetails);
		//ActorHierarchyDetails->UpdateWidget();
		//UE_LOG(LogTemp, Warning, TEXT("ActorHierarchySizeDetails size: %d"), ActorHierarchyDetails->TextForTextBlocks.Num());
	}

	UpdateTextBlocks(UpdateInfo, HierarchySizeDetails);
}

/** Checks each actor in level to see if it is attached to a blueprint; returns -1 if InLevel is null*/
int32 UAssetInsightWidget::GetNumBlueprintsInLevel(const ULevel* InLevel) 
{
	int32 numOfBlueprints = -1;

	if (InLevel) 
	{
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
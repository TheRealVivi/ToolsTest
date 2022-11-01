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
		FileSizeText->SetText(FText::FromString(TEXT("File Size: <No asset selected>")));
	}

	if (NumOfActorsText) 
	{
		NumOfActorsText->SetText(FText::FromString(TEXT("# Actors: <No asset selected>")));
	}

	if (NumOfBlueprintsText)
	{
		NumOfBlueprintsText->SetText(FText::FromString(TEXT("# Blueprints: <No asset selected>")));
	}

	if (MapSizeText) 
	{
		MapSizeText->SetText(FText::FromString(TEXT("Cartesean Size: <No asset selected>")));
	}

	if (ActorHierarchyDetails) 
	{
		ActorHierarchyDetails->SetText(FText::FromString(TEXT("<No asset selected - Select a level for details>")));
	}

	if (ActorHierarchyRelationship) 
	{
		ActorHierarchyRelationship->SetText(FText::FromString(TEXT("<No asset selected - Select a level for details>")));
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
void UAssetInsightWidget::UpdateTextBlocks(TArray<FString> InUpdates, TArray<FString>* InHierarchyUpdates, TArray<FString>* InHierarchyRelationship) 
{
	// Update member TextBlock widget text with selected asset insights
	if (AssetNameText) 
	{
		AssetNameText->SetText(FText::FromString("Asset Name: " + InUpdates[ASSET_NAME]));
	}

	if (FileSizeText)
	{
		FileSizeText->SetText(FText::FromString("File Size: " + InUpdates[FILE_SIZE] + " bytes"));
	}

	if (NumOfActorsText)
	{
		NumOfActorsText->SetText(FText::FromString("# Actors: " + InUpdates[ACTOR_TEXT]));
	}

	if (NumOfBlueprintsText)
	{
		NumOfBlueprintsText->SetText(FText::FromString("# Blueprints: " + InUpdates[BLUEPRINT_TEXT]));
	}

	if (MapSizeText) 
	{
		MapSizeText->SetText(FText::FromString("Cartesean Size: " + InUpdates[MAP_SIZE]));
	}

	if (ActorHierarchyDetails) 
	{
		ActorHierarchyDetails->SetText(FText::FromString(UpdateHierarchyDetails(InHierarchyUpdates)));
	}

	if (ActorHierarchyRelationship) 
	{
		ActorHierarchyRelationship->SetText(FText::FromString(UpdateHierarchyRelationship(InHierarchyRelationship)));
	}
}

FString UAssetInsightWidget::UpdateHierarchyDetails(TArray<FString>* InHierarchyUpdates) 
{
	FString HierarchyDetails = "";

	if (!InHierarchyUpdates->IsEmpty()) 
	{
		for (FString Detail : *InHierarchyUpdates)
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

FString UAssetInsightWidget::UpdateHierarchyRelationship(TArray<FString>* InHierarchyRelationship) 
{
	FString HierarchyDetails = "";

	if (!InHierarchyRelationship->IsEmpty()) 
	{
		for (FString Relationship : *InHierarchyRelationship) 
		{
			HierarchyDetails += Relationship + "\n";
		}
	}
	else 
	{
		HierarchyDetails = "No hierarchy relationship to display.";
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

TMap<FString, TArray<FString>> UAssetInsightWidget::CalculateLevelHierarchyRelationship(const ULevel* InLevel) 
{
	TMap<FString, TArray<FString>> HierarchyRelationships;
	TArray<FString> HierarchyRelationship;
	// Calculate hierarchy bounds using folders
	if (InLevel->IsUsingActorFolders())
	{
		// use a TMap<Folder Name, Bounds> to calculate hierarchy bounds

		for (AActor* Actor : InLevel->Actors)
		{
			FString ActorFolderName = Actor->GetFolder().ToString();
			//UE_LOG(LogTemp, Warning, TEXT("Actor name: %s, Actor folder path: %s"), *Actor->GetActorNameOrLabel(), *Actor->GetFolderPath().ToString());
			if (!HierarchyRelationships.Contains(ActorFolderName))
			{
				HierarchyRelationships.Add(ActorFolderName);
				HierarchyRelationships[ActorFolderName].Add(Actor->GetActorNameOrLabel() + " " + Actor->GetActorLocation().ToString());
				//UE_LOG(LogTemp, Warning, TEXT("Newly added Folder name: %s New bounds size: %s"), *ActorFolderName, *HierarchyBounds[ActorFolderName].GetSize().ToString());
			}
			else
			{
				HierarchyRelationships[ActorFolderName].Add(Actor->GetActorNameOrLabel() + " " + Actor->GetActorLocation().ToString());
				//UE_LOG(LogTemp, Warning, TEXT("Existing Folder name: %s New bounds size: %s"), *ActorFolderName, *HierarchyBounds[ActorFolderName].GetSize().ToString());
			}

			//UE_LOG(LogTemp, Warning, TEXT("Folder name: %s"), *Actor->GetFolder().ToString());
		}

		for (TPair<FString, TArray<FString>> HierarchyPair : HierarchyRelationships)
		{
			FString Relationship = "";
			for (int32 i = 0; i < HierarchyPair.Value.Num(); i++)
			{
				if (i == 0)
				{
					Relationship += HierarchyPair.Key + ":\n";
				}

				Relationship += "  -" + HierarchyPair.Value[i] + "\n";
			}
			HierarchyRelationship.Add(Relationship);
		}

		HierarchyRelationship.StableSort();
	}

	return HierarchyRelationships;
}



/** 
 *  TODO: Pull actor info from External packages to cover external package user case
*/
void UAssetInsightWidget::UpdateInsights()
{
	TArray<FAssetData> SelectedObjectsAssetData = UEditorUtilityLibrary::GetSelectedAssetData();
	
	if (SelectedObjectsAssetData.Num() != 1)
		return;

	TArray<FString> UpdateInfo;
	TArray<FString> HierarchySizeDetails;
	TArray<FString> HierarchyRelationship;
	FString NumOfActors = "<Select a level>";
	FString NumOfBlueprints = "<Select a level>";
	FString LevelSize = "<Select a level>";

	FAssetData SelectedAssetData = SelectedObjectsAssetData[0];
	UWorld* World = Cast<UWorld>(SelectedAssetData.GetAsset());

	if (World)
	{
		ULevel* Level = World->GetCurrentLevel();

		if(Level->IsUsingExternalActors()) 
		{
			UE_LOG(LogTemp, Warning, TEXT("Is using external actors"));
			UE_LOG(LogTemp, Warning, TEXT("Num of actors from external package: %d"), Level->Actors.Num());
			
		}			

		NumOfActors = FString::FromInt(Level->Actors.Num());
		NumOfBlueprints = FString::FromInt(GetNumBlueprintsInLevel(Level));
		LevelSize = GetLevelBounds(Level, &SelectedAssetData).ToString();
		GetActorHierarchyFootprint(Level, &HierarchySizeDetails);
		GetActorHierarchyRelationship(Level, &HierarchyRelationship);
	}

	UpdateInfo.Add(*SelectedAssetData.AssetName.ToString());
	UpdateInfo.Add(FString::FromInt(SelectedAssetData.GetPackage()->GetFileSize()));
	UpdateInfo.Add(NumOfActors);
	UpdateInfo.Add(NumOfBlueprints);
	UpdateInfo.Add(LevelSize);

	UpdateTextBlocks(UpdateInfo, &HierarchySizeDetails, &HierarchyRelationship);
}

void UAssetInsightWidget::GetActorHierarchyRelationship(const ULevel* InLevel, TArray<FString>* OutHierarchyRelationship) 
{
	TMap<FString, TArray<FString>> HierarchyRelationships = CalculateLevelHierarchyRelationship(InLevel);
	
	for (TPair<FString, TArray<FString>> HierarchyPair : HierarchyRelationships)
	{
		FString Relationship = "";
		for (int32 i = 0; i < HierarchyPair.Value.Num(); i++)
		{
			if (i == 0)
			{
				Relationship += HierarchyPair.Key + ":\n";
			}
			
			Relationship += "  -" + HierarchyPair.Value[i] + "\n";
		}

		OutHierarchyRelationship->Add(Relationship);
	}

	OutHierarchyRelationship->StableSort();
}

void UAssetInsightWidget::GetActorHierarchyFootprint(const ULevel* InLevel, TArray<FString>* OutHierarchyFootprint) 
{
	TMap<FString, FBox> HierarchyBounds = CalculateLevelHierarchyBounds(InLevel);

	for (TPair<FString, FBox> HierarchyBoundPair : HierarchyBounds)
	{
		OutHierarchyFootprint->Add(HierarchyBoundPair.Key + ": " + HierarchyBoundPair.Value.GetSize().ToString());
	}
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
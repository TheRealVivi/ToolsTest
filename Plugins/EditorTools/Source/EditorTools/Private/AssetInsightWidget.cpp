// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetInsightWidget.h"
#include "Components/TextBlock.h"
#include "HorizontalExpandingListWidget.h"
#include "EditorUtilityLibrary.h"
#include "Runtime/AssetRegistry/Public/AssetRegistry/AssetRegistryModule.h"

void UAssetInsightWidget::NativeConstruct() 
{
	Super::NativeConstruct();

	bIncludeNonColliding = true;

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

	
}

/**
 *  TODO: Pull actor info from External packages to cover external package user case
*/
void UAssetInsightWidget::UpdateInsights(bool bInIncludeNonColliding)
{
	bIncludeNonColliding = bInIncludeNonColliding;

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

		/* Attempting to gather external actor details
		 * Able to get total number of external actors when not loaded, 
		 * Searching how to load these details into an actor and gather more info
		 */
		/*
		int32 NumExternalActors = 0;
		if (Level->IsUsingExternalActors())
		{	
			TArray<FString> ExternalActorPacks = Level->GetOnDiskExternalActorPackages();
			TArray<FName> ExternalActorPaths;
			for (FString ExternalActorPack : ExternalActorPacks) 
			{
				ExternalActorPaths.Add(FName(*ExternalActorPack));
			}

			for (FName ExternalActorPath : ExternalActorPaths) 
			{
				UE_LOG(LogTemp, Warning, TEXT("Actor path: %s"), *ExternalActorPath.ToString());
			}

			//TArray<FAssetData> AssetData;
			//UAssetRegistryImpl AssetRegistry = UAssetRegistryImpl::Get();
			//FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
			//AssetRegistryModule.Get().GetAssetsByPaths(ExternalActorPaths, AssetData);
			
			//AssetRegistryModule.Get().GetAllAssets(AssetData);
		
			//UAssetRegistryImpl::Get().GetAssetsByPaths(ExternalActorPaths, *AssetData);
			//IAssetRegistry::Get()->GetAssetByObjectPath(ExternalActorPaths, *AssetData);
			//bool test = AssetRegistry->GetAssetsByPaths(ExternalActorPaths, *AssetData, true, false);

			
			UE_LOG(LogTemp, Warning, TEXT("AssetData size: %d"), AssetData.Num());

			for (FAssetData aData : AssetData) 
			{
				AActor* Actor = Cast<AActor>(aData.GetAsset());
				if (Actor) {
					UE_LOG(LogTemp, Warning, TEXT("Actor name: %s"), *Actor->GetActorNameOrLabel());
				}
			}

			//NumExternalActors += ExternalActorPacks.Num();
		}
		*/
	
		

		NumOfActors = FString::FromInt(Level->Actors.Num());
		NumOfBlueprints = FString::FromInt(GetNumBlueprintsInLevel(Level));
		LevelSize = GetLevelSize(Level).ToString();
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

/**
 * \Brief:   Prepares string with desired formatting for ActorHierarchyDetails TextBlock
 * \Returns: FString desired formatting for ActorHierarchyDetails TextBlock
 */
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

/**
 * \Brief:   Prepares string with desired formatting for ActorHierarchyRelationship TextBlock
 * \Returns: FString desired formatting for ActorHierarchyRelationship TextBlock
 */
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

/**
 * \Note:    Copy/paste from FLevelBounds class.
 * \Brief:   Calculates level bounds
 *           If bIncludeNonColliding is true, will add bounding boxes of non-colliding actors
 * \Returns: An FBox with all of the level actors bounding boxes summed up
*/
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
				FBox ActorBox = Actor->GetComponentsBoundingBox(bIncludeNonColliding);
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
 * \Brief:   Uses a TMap to map out the Level's Actor hierarchy
 * \Returns: TMap with Level's Actor hierarchy and summed up bounding boxes
*/
TMap<FString, FBox> UAssetInsightWidget::CalculateLevelHierarchyBounds(const ULevel* InLevel)
{
	TMap<FString, FBox> HierarchyBounds;

	for (AActor* Actor : InLevel->Actors)
	{
		if (Actor && Actor->IsLevelBoundsRelevant()) 
		{
			FString ActorFolderName = Actor->GetFolder().ToString();
			if (!HierarchyBounds.Contains(ActorFolderName))
			{
				HierarchyBounds.Add(ActorFolderName, Actor->GetComponentsBoundingBox(bIncludeNonColliding));
			}
			else
			{
				HierarchyBounds[ActorFolderName] += Actor->GetComponentsBoundingBox(bIncludeNonColliding);
			}
		}
	}

	return HierarchyBounds;
}

/**
 * \Brief:   Uses a TMap to map out the Level's Actor hierarchy relationship
 * \Returns: TMap with Level's Actor hierarchy relationships
*/
TMap<FString, TArray<FString>> UAssetInsightWidget::CalculateLevelHierarchyRelationship(const ULevel* InLevel) 
{
	TMap<FString, TArray<FString>> HierarchyRelationships;
	TArray<FString> HierarchyRelationship;

	for (AActor* Actor : InLevel->Actors)
	{
		if (Actor) 
		{
			FString ActorFolderName = Actor->GetFolder().ToString();

			if (!HierarchyRelationships.Contains(ActorFolderName))
			{
				HierarchyRelationships.Add(ActorFolderName);
				HierarchyRelationships[ActorFolderName].Add(FString::Printf(TEXT("%s\t\t\t\t\t%s"), *Actor->GetActorNameOrLabel(), *Actor->GetActorLocation().ToString()));
			}
			else
			{
				HierarchyRelationships[ActorFolderName].Add(FString::Printf(TEXT("%s\t\t\t\t\t%s"), *Actor->GetActorNameOrLabel(), *Actor->GetActorLocation().ToString()));
			}
		}
	}
	
	return HierarchyRelationships;
}

/**
 * \Brief:   Uses a TMap to map out the Level's Actor hierarchy
 *           Given TArray reference will be filled with the hierarchy relationships
*/
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
			
			Relationship += "  - " + HierarchyPair.Value[i] + "\n";
		}

		OutHierarchyRelationship->Add(Relationship);
	}

	OutHierarchyRelationship->StableSort();
}

/**
 * \Brief:   Uses a TMap to map out the Level's Actor hierarchy,
 *           Given TArray reference will be filled with hierarchy and it's size
*/
void UAssetInsightWidget::GetActorHierarchyFootprint(const ULevel* InLevel, TArray<FString>* OutHierarchyFootprint) 
{
	TMap<FString, FBox> HierarchyBounds = CalculateLevelHierarchyBounds(InLevel);

	for (TPair<FString, FBox> HierarchyBoundPair : HierarchyBounds)
	{
		OutHierarchyFootprint->Add(HierarchyBoundPair.Key + ": " + HierarchyBoundPair.Value.GetSize().ToString());
	}
}


/** Checks if level's asset data has tag values for level bounds, if not, calculates them
 *  Returns level bounds as a FVector3d
*/
FVector3d UAssetInsightWidget::GetLevelSize(const ULevel* InLevel)
{
	FVector3d levelSize;
	FBox levelBounds;

	if (InLevel) 
	{
		levelBounds = CalculateLevelBounds(InLevel);
	}

	levelSize = levelBounds.GetSize();

	return levelSize;
}

/**
 * \Brief:   Sums up number of blueprints referenced InLevel
 * \Returns: Returns number of blueprints. Will return -1 if InLevel is null
*/
int32 UAssetInsightWidget::GetNumBlueprintsInLevel(const ULevel* InLevel)
{
	int32 numOfBlueprints = -1;

	if (InLevel)
	{
		numOfBlueprints = 0;
		TArray<AActor*> LevelActors = InLevel->Actors;

		for (AActor* Actor : LevelActors)
		{
			if (Actor) 
			{
				if (Actor->GetArchetype()->IsInBlueprint())
				{
					numOfBlueprints++;
				}
			}
		}
	}

	return numOfBlueprints;
}

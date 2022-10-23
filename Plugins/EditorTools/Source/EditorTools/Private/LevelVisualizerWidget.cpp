// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelVisualizerWidget.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/TextBlock.h"
#include "AssetInsightWidget.h"
#include "EditorUtilityLibrary.h"

#define DEBUG_MSG(x, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT(x), __VA_ARGS__));}

void ULevelVisualizerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_0) 
	{
		Button_0->OnClicked.AddDynamic(this, &ThisClass::UpdateInsights);
		Button_0->SetRenderOpacity(.2f);
	}

	if (SomeText) 
	{
		SomeText->SetText(FText::FromString(TEXT("Hello world! lolol")));
		SomeText->SetOpacity(.1f);
	}

	
}

void ULevelVisualizerWidget::TestFunction()
{
	UE_LOG(LogTemp, Warning, TEXT("Hello"));
}

void ULevelVisualizerWidget::UpdateInsights()
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
	
	if (InsightWidget) 
	{
		InsightWidget->UpdateInsights(UpdateInfo);
	}
}

FBox ULevelVisualizerWidget::CalculateLevelBounds(const ULevel* InLevel)
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
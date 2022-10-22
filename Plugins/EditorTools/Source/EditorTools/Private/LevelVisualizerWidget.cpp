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

	//FVector worldCenter = world->GetModel()->GetCenter();
	if (SelectedObjectsAssetData.Num() == 1) 
	{
		FAssetData SelectedAssetData = SelectedObjectsAssetData[0];
		SelectedAssetData.PrintAssetData();
		UWorld* world = Cast<UWorld>(SelectedAssetData.GetAsset());
		int32 numOfActors, numOfBlueprints = 0;

		if (world) 
		{
			TArray<AActor*> LevelActors = world->GetCurrentLevel()->Actors;
			numOfActors = LevelActors.Num();



			for (AActor* a : LevelActors)
			{
				if (a->GetFName().ToString().StartsWith("BP_") || a->GetFName().ToString().StartsWith("Blueprint_"))
					numOfBlueprints++;
			}
		}

		UpdateInfo.Add(*SelectedAssetData.AssetName.ToString());
		UpdateInfo.Add(FString::FromInt(SelectedAssetData.GetPackage()->GetFileSize()).Append(" bytes"));
		UpdateInfo.Add(FString::FromInt(numOfActors));
		UpdateInfo.Add(FString::FromInt(numOfBlueprints));
	}
	
	if (InsightWidget) 
	{
		InsightWidget->UpdateInsights(UpdateInfo);
	}
	
}
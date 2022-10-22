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


	for (FAssetData SelectedAssetData : SelectedObjectsAssetData) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Asset Class Type: %s"), *SelectedAssetData.AssetClass.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Asset Name: %s"), *SelectedAssetData.AssetName.ToString());
		UWorld* world = Cast<UWorld>(SelectedAssetData.GetAsset());
		if (world) 
		{
			//FVector worldCenter = world->GetModel()->GetCenter();
			UE_LOG(LogTemp, Warning, TEXT("Actor count in world: %d"), world->GetActorCount());
			UE_LOG(LogTemp, Warning, TEXT("Level actor count: %d"), world->GetCurrentLevel()->Actors.Num());
			//UE_LOG(LogTemp, Warning, TEXT("World Center vector: %f %f"), worldCenter.X, worldCenter.Y);
		}
		SelectedAssetData.PrintAssetData();

		UpdateInfo.Add(*SelectedAssetData.AssetName.ToString());
		UpdateInfo.Add(FString::FromInt(SelectedAssetData.GetPackage()->GetFileSize()).Append(" bytes"));
	}


	
	if (InsightWidget) 
	{
		InsightWidget->UpdateInsights(UpdateInfo);
	}
	
}
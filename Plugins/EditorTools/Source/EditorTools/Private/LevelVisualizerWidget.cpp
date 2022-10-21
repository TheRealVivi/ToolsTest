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

	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();
	TArray<FString> UpdateInfo;

	FString objectDescription;
	TMap<FName, UObject::FAssetRegistryTagMetadata> metadata;

	for (UObject* SelectedObject : SelectedObjects) 
	{

		ULevel* TestLevel = (ULevel*)SelectedObject;
		//int32 numOfActors;
		TArray<UObject*> subobjects;
		TestLevel->GetDefaultSubobjects(subobjects);

		objectDescription = SelectedObject->GetDesc();
		SelectedObject->GetAssetRegistryTagMetadata(metadata);
		UE_LOG(LogTemp, Warning, TEXT("Desc: %s"), *SelectedObject->GetDesc());
		UE_LOG(LogTemp, Warning, TEXT("Detailed Info: %s"), *SelectedObject->GetDetailedInfo());
		UE_LOG(LogTemp, Warning, TEXT("Object name: %s"), *SelectedObject->GetFName().ToString()); // Literally prints assets name
		UE_LOG(LogTemp, Warning, TEXT("File Size: %d"), SelectedObject->GetPackage()->GetFileSize());
		UE_LOG(LogTemp, Warning, TEXT("Num of level blueprints: %d"), TestLevel->GetLevelBlueprints().Num());
		UE_LOG(LogTemp, Warning, TEXT("Num of Actors: %d"), TestLevel->Actors.Num()); // fix
		UE_LOG(LogTemp, Warning, TEXT("Num of Actors: %d"), TestLevel->ActorsForGC.Num()); // fix
		UE_LOG(LogTemp, Warning, TEXT("Num of Actors: %d"), subobjects.Num());
		//UE_LOG(LogTemp, Warning, TEXT("Num of level script blueprints: %d"), TestLevel->GetLevelScriptBlueprint().Num());

		UpdateInfo.Add(*SelectedObject->GetFName().ToString());
		UpdateInfo.Add(FString::FromInt(SelectedObject->GetPackage()->GetFileSize()).Append(" bytes"));
	}

	/*
	for (const TPair<FName, UObject::FAssetRegistryTagMetadata>& pair : metadata) 
	{
		if (InsightWidget)
		{
			//InsightWidget->UpdateInsights(FText::FromString(pair.Key.ToString()));
			InsightWidget->UpdateInsights(FText::FromString(pair.Value.ImportantValue));
		}

		FString test = pair.Key.ToString();

		
		UE_LOG(LogTemp, Warning, TEXT("Key is %s"), *test);
		UE_LOG(LogTemp, Warning, TEXT("Value Name is %s"), *pair.Value.DisplayName.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Value is %s"), *pair.Value.ImportantValue);
		UE_LOG(LogTemp, Warning, TEXT("Value suffix is %s"), *pair.Value.Suffix.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Value tooltip text is %s"), *pair.Value.TooltipText.ToString());
		
		//UE_LOG(LogTemp, Warning, TEXT("hey"));
	}
	*/

	
	if (InsightWidget) 
	{
		InsightWidget->UpdateInsights(UpdateInfo);
	}
	
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "LevelVisualizerWidget.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class EDITORTOOLS_API ULevelVisualizerWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
public:
	ULevelVisualizerWidget();
	~ULevelVisualizerWidget();

	UFUNCTION(BlueprintCallable)
	void TestFunction();
};

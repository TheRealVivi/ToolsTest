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
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UButton* AnalyzeButton;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UCheckBox* IncludeNonCollidingCheckBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UAssetInsightWidget* InsightWidget;

	UFUNCTION(BlueprintCallable)
	void UpdateInsightWidget();
};

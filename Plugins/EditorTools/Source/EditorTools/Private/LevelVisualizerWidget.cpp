// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelVisualizerWidget.h"

#define DEBUG_MSG(x, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT(x), __VA_ARGS__));}

ULevelVisualizerWidget::ULevelVisualizerWidget() 
{
}

ULevelVisualizerWidget::~ULevelVisualizerWidget() 
{
}

void ULevelVisualizerWidget::TestFunction()
{
	DEBUG_MSG("Hello World!")
}

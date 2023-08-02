// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusWidget.generated.h"

/**
 * 
 */
class UTextBlock;
class UProgressBar;

UCLASS()
class PROJECTZ_API UStatusWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Meta=(BindWidget))
	UTextBlock* TextBlock_speedNum;

	UPROPERTY(Meta=(BindWidget))
	UTextBlock* TextBlock_Dist;

	UPROPERTY(Meta=(BindWidget))
	UTextBlock* TextBlock_Feet;

	UPROPERTY(Meta=(BindWidget))
	UTextBlock* TextBlock_entryTime;

	UPROPERTY(Meta=(BindWidget))
	UTextBlock* TextLevel;

	UPROPERTY(Meta=(BindWidget))
	UTextBlock* TextName;

	UPROPERTY(Meta=(BindWidget))
	UTextBlock* TextCadence;

	UPROPERTY(Meta=(BindWidget))
	UTextBlock* TextBlock_kmDP;

	UPROPERTY(Meta=(BindWidget))
	UTextBlock* TextBlock_MapName;
	
	UPROPERTY(Meta=(BindWidget))
	UProgressBar* ProgressBar_level;

	UPROPERTY(Meta=(BindWidget))
	UProgressBar* ProgressBar_Dist;
	
public:
	void Test();
	void InitSettingDp();
	void UpdateBicycleStatus();
};

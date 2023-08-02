// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ZInGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API AZInGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	AZInGameHUD();
	
public:
	UPROPERTY(EditAnywhere, Category = "Widget")
		TSubclassOf<UUserWidget> InGameWidgetClass;

	UPROPERTY(EditAnywhere, Category="MinimapWidget")
	TSubclassOf<UUserWidget> MinimapWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UUserWidget* InGameWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UUserWidget* MinimapWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SplineName;
};

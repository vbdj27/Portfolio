// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API UInGameInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDirSelectWidget* DirSelectWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWattInfoWidget* WattInfoWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStatusWidget* StatusWidget;
	
	
public:
	virtual void NativeConstruct() override;
};

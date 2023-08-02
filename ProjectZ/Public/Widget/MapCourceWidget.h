// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Commons/Commons.h"
#include "MapCourceWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API UMapCourceWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FString LocalServerURL;

	UPROPERTY(Meta = (BindWidget))
	class UButton* Button_R1;

	UPROPERTY(Meta = (BindWidget))
	class UButton* Button_R2;

	UPROPERTY(Meta = (BindWidget))
	class UButton* Button_R3;

	UPROPERTY(Meta = (BindWidget))
	class UButton* Button_Start;

	FString StartingPoint;
	
public:
	UMapCourceWidget(const FObjectInitializer& ObjectInitializer);
	
	
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void OnClickedR1();

	UFUNCTION(BlueprintCallable)
	void OnClickedR2();

	UFUNCTION(BlueprintCallable)
	void OnClickedR3();

	UFUNCTION(BlueprintCallable)
	void OnClickedStart();
	
	void OnClickedOpenLevel(ELevelName Name);
};

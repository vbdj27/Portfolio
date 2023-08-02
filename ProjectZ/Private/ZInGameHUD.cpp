// Fill out your copyright notice in the Description page of Project Settings.


#include "ZInGameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Widget/MinimapWidget.h"


AZInGameHUD::AZInGameHUD()
{

}

//困连 按眉 积己
void AZInGameHUD::BeginPlay()
{
	Super::BeginPlay();

	if (InGameWidgetClass != nullptr)
	{
		InGameWidget = CreateWidget(GetWorld(), InGameWidgetClass);

		if (InGameWidget != nullptr)
		{
			InGameWidget->AddToViewport();
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;
		}

		else
		{
			UE_LOG(LogTemp, Warning, TEXT("InGameWidget is Not Found!!"));
		}

	}

	else 
	{
		UE_LOG(LogTemp,Warning, TEXT("InGameWidgetClass is Not Found!!"));
	}

 	if(MinimapWidgetClass != nullptr)
 	{
 		MinimapWidget = CreateWidget(GetWorld(), MinimapWidgetClass);

 		if(MinimapWidget != nullptr)
 		{
 			Cast<UMinimapWidget>(MinimapWidget)->CallInit();
 			MinimapWidget->AddToViewport();
 		}
 	}
}
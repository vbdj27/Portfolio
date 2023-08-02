// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Commons/Commons.h"
#include "LobbyUserWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class PROJECTZ_API ULobbyUserWidget : public UUserWidget
{
	GENERATED_BODY()
	ULobbyUserWidget(const FObjectInitializer& ObjectInitializer);

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	FString CityParkUrl;
	FString SFUrl;

	UPROPERTY(Meta=(BindWidget))
	UButton* Button_NormalRoute;
	
	UPROPERTY(Meta=(BindWidget))
	UButton* Button_CircleRoute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AMainMenuHUD* MainMenuHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UZGameInstance* ZGameInstance;

	void OnClickedOpenLevel(ELevelName Name);

	UFUNCTION()
	void OnClickedDefaultRoute();

	UFUNCTION()
	void OnClickedCircleRoute();

	UFUNCTION()
	void OnClickedSfRoute();
};

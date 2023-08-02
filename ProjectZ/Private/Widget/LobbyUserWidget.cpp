// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/LobbyUserWidget.h"

#include "TextReaderComponent.h"
#include "ZGameInstance.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "MainMenuHUD.h"
#include "ZPlayerController.h"

ULobbyUserWidget::ULobbyUserWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	UTextReaderComponent* TextReader = CreateDefaultSubobject<UTextReaderComponent>(TEXT("TextReader"));

	CityParkUrl = TextReader->ReadFile("Urls/TestUrl.txt");
	SFUrl = TextReader->ReadFile("Urls/SFUrl.txt");
	
}

void ULobbyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ZGameInstance = Cast<UZGameInstance>(GetGameInstance());

	FScriptDelegate DefaultRouteDelegate;
	DefaultRouteDelegate.BindUFunction(this, "OnClickedDefaultRoute");
	Button_NormalRoute->OnClicked.Add(DefaultRouteDelegate);

	FScriptDelegate CircleRouteDelegate;
	CircleRouteDelegate.BindUFunction(this, "OnClickedCircleRoute");
	Button_CircleRoute->OnClicked.Add(CircleRouteDelegate);

	MainMenuHUD = Cast<AMainMenuHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());

}

void ULobbyUserWidget::NativeDestruct()
{

	Super::NativeDestruct();

	
}

void ULobbyUserWidget::OnClickedOpenLevel(ELevelName Name)
{
	
	if(ZGameInstance!= nullptr)
	{
		//route값 전달 index
		//ZGameInstance->ShowLoadingWidgetAfterLoadLevel("CityPark");
		ZGameInstance->LoadingAfterLoadLevel(Name);
	
	}
}

void ULobbyUserWidget::OnClickedDefaultRoute()
{
	UE_LOG(LogTemp,Warning,TEXT("OnClickedDefaultRoute"));
	//OnClickedOpenLevel(ELevelName::ECityPark);
	//OnClickedOpenLevel(ELevelName::ETest);
	
	MainMenuHUD->ShowWidget(MainMenuHUD->MapCourseWidget);
	
	ZGameInstance->CurrentLevel = "CityPark";
}

void ULobbyUserWidget::OnClickedCircleRoute()
{
	UE_LOG(LogTemp,Warning,TEXT("OnClickedCircleRoute"));
	//OnClickedOpenLevel(ELevelName::ESF);
	MainMenuHUD->ShowWidget(MainMenuHUD->MapCourseWidget);

	ZGameInstance->CurrentLevel = "SF";
}

void ULobbyUserWidget::OnClickedSfRoute()
{
	
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/InGameInfoWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Widget/DirSelectWidget.h"
#include "Widget/StatusWidget.h"
#include "Widget/WattInfoWidget.h"

//위젯 가져오기
void UInGameInfoWidget::NativeConstruct()
{
	UUserWidget* DirSelectWidgetDummy = this->WidgetTree->FindWidget<UUserWidget>("WBP_DirSelect");
	UUserWidget* WattInfoWidgetDummy = this->WidgetTree->FindWidget<UUserWidget>("WBP_WattInfo");
	UUserWidget* StatusWidgetDummy = this->WidgetTree->FindWidget<UUserWidget>("WBP_Status");

	DirSelectWidget = Cast<UDirSelectWidget>(DirSelectWidgetDummy);
	if(DirSelectWidget)
	{
		DirSelectWidget->Test();
	}

	WattInfoWidget = Cast<UWattInfoWidget>(WattInfoWidgetDummy);
	if(WattInfoWidget)
	{
		WattInfoWidget->Test();
	}

	StatusWidget = Cast<UStatusWidget>(StatusWidgetDummy);
	if(StatusWidget)
	{
		StatusWidget->Test();
		StatusWidget->InitSettingDp();
	}
}

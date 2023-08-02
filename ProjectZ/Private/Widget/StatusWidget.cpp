// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/StatusWidget.h"

#include "ZGameInstance.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UStatusWidget::Test()
{
	UE_LOG(LogTemp,Warning,TEXT("StatusWidget Test Call!"));

}

//유저 셋팅 정보
void UStatusWidget::InitSettingDp()
{
	UZGameInstance* ZGameInstance = Cast<UZGameInstance>(GetGameInstance());
	if(ZGameInstance)
	{
		TextName->SetText(FText::FromString(ZGameInstance->GetAvatarInfo().UserName));
		TextLevel->SetText(FText::FromString(FString::FromInt(ZGameInstance->GetAvatarInfo().Level)));
		TextBlock_MapName->SetText(FText::FromString(UGameplayStatics::GetCurrentLevelName(GetWorld())));
	}
}

//라이딩 정보 업데이트
void UStatusWidget::UpdateBicycleStatus()
{
	UZGameInstance* ZGameInstance = Cast<UZGameInstance>(GetGameInstance());
	if(ZGameInstance)
	{
		TextBlock_speedNum->SetText(FText::FromString(ZGameInstance->BicycleCalcData.PlayerSpeed));
		TextBlock_Dist->SetText(FText::FromString(ZGameInstance->BicycleCalcData.TotalKm));
	}
}

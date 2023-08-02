// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UWebBrowser;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class PROJECTZ_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	//웹브라우저
	UPROPERTY(Meta=(BindWidget))
	UWebBrowser* WebBrowserLogin;

	UPROPERTY(Meta=(BindWidget))
	UTextBlock* Text_Title;
		
	UPROPERTY(Meta=(BindWidget))
	UButton* Button_Join;

	UPROPERTY(Meta=(BindWidget))
	UButton* Button_Inventory;

	
private:
	//Http 통신 모듈
	FHttpModule* HttpModule;

	UPROPERTY()
	FString LoginUrl;

	UPROPERTY()
	FString ApiUrl;

	UPROPERTY()
	FString CallbackUrl;

public:

	///
	///로그인 이벤트
	///
	UFUNCTION()
	void HandleLoginUrlChange();

	UFUNCTION()
	void OnLoginSettinUsername(FString username);

	///
	///플레이어 데이터 리시브
	///
	void OnGetPlayerDataResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response,
													  bool bWasSuccessFul);
	///
	///토큰 교환 리시브
	///
	void OnExchangeCodeForTokensResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response,
													  bool bWasSuccessFul);
	///
	///조인 버튼 클릭 이벤트
	///
	UFUNCTION()
	void OnClickedJoin();

	UFUNCTION()
	void OnClickedInventory();
};

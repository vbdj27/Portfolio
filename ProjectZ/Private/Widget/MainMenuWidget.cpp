// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MainMenuWidget.h"

#include "WebBrowser.h"
#include "IWebBrowserCookieManager.h"
#include "IWebBrowserSingleton.h"
#include "MainMenuHUD.h"
#include "RestApiGameInstanceSubSystem.h"
#include "TextReaderComponent.h"
#include "WebBrowserModule.h"
#include "WebSocketGameInstanceSubSystem.h"
#include "ZGameInstance.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Interfaces/IHttpResponse.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	//텍스트 리더로 txt파일 읽어오기 공통부분
	UTextReaderComponent* TextReader = CreateDefaultSubobject<UTextReaderComponent>(TEXT("TextReaderComp"));

	LoginUrl = TextReader->ReadFile("Urls/LoginUrl.txt");
	ApiUrl = TextReader->ReadFile("Urls/ApiUrl.txt");
	CallbackUrl = TextReader->ReadFile("Urls/CallbackUrl.txt");
	HttpModule = &FHttpModule::Get();
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//OnClickedJoin 버튼이벤트
	FScriptDelegate Button_JoinDelegate;
	Button_JoinDelegate.BindUFunction(this, "OnClickedJoin");
	Button_Join->OnClicked.Add(Button_JoinDelegate);

	//OnClickedInventory 버튼이벤트
	FScriptDelegate Button_InventoryDelegate;
	Button_InventoryDelegate.BindUFunction(this, "OnClickedInventory");
	Button_Inventory->OnClicked.Add(Button_InventoryDelegate);

	
	FString AccessToken;
	UWebSocketGameInstanceSubSystem* WSGS = GetGameInstance()->GetSubsystem<UWebSocketGameInstanceSubSystem>();
	if (WSGS != nullptr)
	{
		URestApiGameInstanceSubSystem* RAGS = GetGameInstance()->GetSubsystem<URestApiGameInstanceSubSystem>();
		if (RAGS != nullptr)
		{
			AccessToken = RAGS->AccessToken;
		}

		//WSGS->Dele_LoginCompleteUsernameSetting_OneParam.BindUFunction(this, FName("OnLoginSettinUsername"));
	}

	if (AccessToken.Len() > 0)
	{
		//토큰이 발행되었을 경우
		UE_LOG(LogTemp,Warning,TEXT("AT Have"));
		
		// TSharedRef<IHttpRequest> GetPlayerDataRequest = HttpModule->CreateRequest();
		// GetPlayerDataRequest->OnProcessRequestComplete().BindUObject(this, &UMainMenuWidget::OnGetPlayerDataResponseReceived);
		// GetPlayerDataRequest->SetURL(ApiUrl + "/getplayerdata");
		// GetPlayerDataRequest->SetVerb("GET");
		// GetPlayerDataRequest->SetHeader("Authorization", AccessToken);
		// GetPlayerDataRequest->ProcessRequest();
		
		WebBrowserLogin->SetVisibility(ESlateVisibility::Hidden);
		Text_Title->SetVisibility(ESlateVisibility::Visible);
		Button_Join->SetVisibility(ESlateVisibility::Visible);
		Button_Inventory->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		//토큰이 없는경우
		UE_LOG(LogTemp,Warning,TEXT("AT Don't Have"));

		//웹브라우저 쿠키 제거 후 로그인Url로 로딩
		IWebBrowserSingleton* WebBrowserSingleton = IWebBrowserModule::Get().GetSingleton();

		if (WebBrowserSingleton != nullptr)
		{
			TOptional<FString> DefualtContext;
			TSharedPtr<IWebBrowserCookieManager> CookieManager = WebBrowserSingleton->GetCookieManager(DefualtContext);
			if (CookieManager.IsValid())
			{
				CookieManager->DeleteCookies();
			}
		}

		WebBrowserLogin->LoadURL(LoginUrl);


		//Url변경시 HandleLoginUrlChange로 이벤트 바인딩
		FScriptDelegate LoginDelegate;
		LoginDelegate.BindUFunction(this, "HandleLoginUrlChange");
		WebBrowserLogin->OnUrlChanged.Add(LoginDelegate);

		// float InFloat = 10.0f;
		// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red,  FString::Printf(TEXT("%f"),InFloat));
	}
}

void UMainMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UMainMenuWidget::HandleLoginUrlChange()
{
	FString BrowserUrl = WebBrowserLogin->GetUrl();
	FString Url;
	FString QueryParameters;

	//로그인 성공 후 세팅된 데이터를 전달해 토큰반환요청 성공시 OnExchangeCodeForTokensResponseReceived 이벤트 전달
	if(BrowserUrl.Split("?", &Url, &QueryParameters))
	{
		if(Url.Equals(CallbackUrl))
		{
			FString ParameterName;
			FString ParameterValue;

			if(QueryParameters.Split("=", &ParameterName, &ParameterValue))
			{
				if(ParameterName.Equals("code"))
				{
					ParameterValue = ParameterValue.Replace(*FString("#"), *FString(""));

					TSharedPtr<FJsonObject> RequestObj = MakeShareable(new FJsonObject);
					RequestObj->SetStringField(ParameterName, ParameterValue);

					FString RequestBody;
					TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);

					if(FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer))
					{
						TSharedRef<IHttpRequest> ExChangeCodeForTokensRequest = HttpModule->CreateRequest();
						ExChangeCodeForTokensRequest->OnProcessRequestComplete().BindUObject(this, &UMainMenuWidget::OnExchangeCodeForTokensResponseReceived);
						ExChangeCodeForTokensRequest->SetURL(ApiUrl + "/exchangecodefortokens");
						ExChangeCodeForTokensRequest->SetVerb("POST");
						ExChangeCodeForTokensRequest->SetHeader("Content-Type","application/json");
						ExChangeCodeForTokensRequest->SetContentAsString(RequestBody);
						ExChangeCodeForTokensRequest->ProcessRequest();
					}
				}
			}
		}
	}
}

void UMainMenuWidget::OnLoginSettinUsername(FString username)
{
	UE_LOG(LogTemp, Warning, TEXT("user name : %s "), *username);
}

void UMainMenuWidget::OnGetPlayerDataResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessFul)
{
	//데이터 받기 성공 후 웹브라우저 숨기고 조인페이지 노출
	if(bWasSuccessFul)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
		if(FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			if(JsonObject->HasField("playerData"))
			{
				TSharedPtr<FJsonObject> PlayerData = JsonObject->GetObjectField("PlayerData");
				TSharedPtr<FJsonObject> WinsObject = PlayerData->GetObjectField("Wins");
				TSharedPtr<FJsonObject> LossesObject = PlayerData->GetObjectField("Losses");

				FString Wins = WinsObject->GetStringField("N");
				FString Losses = LossesObject->GetStringField("N");

				//logs win and losses

				WebBrowserLogin->SetVisibility(ESlateVisibility::Hidden);
				Text_Title->SetVisibility(ESlateVisibility::Visible);
				Button_Join->SetVisibility(ESlateVisibility::Visible);
				Button_Inventory->SetVisibility(ESlateVisibility::Visible);


				//ws
				UWebSocketGameInstanceSubSystem* WSGS = GetGameInstance()->GetSubsystem<UWebSocketGameInstanceSubSystem>();
				if(WSGS != nullptr)
				{
					if(WSGS->WebSocket->IsConnected())
					{
						WSGS->OnSentMessage(E_WS_Command::Greet, "");
					}
				}
			}
		}
	}
}

void UMainMenuWidget::OnExchangeCodeForTokensResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessFul)
{
	if(bWasSuccessFul)
	{
		//토큰받는 이벤트 성공시 OnGetPlayerDataResponseReceived 이벤트 전달
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
		if(FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			if(JsonObject->HasField("access_token") && JsonObject->HasField("id_token") && JsonObject->HasField("refresh_token"))
			{
				URestApiGameInstanceSubSystem* RAGS = GetGameInstance()->GetSubsystem<URestApiGameInstanceSubSystem>();
				if(RAGS!=nullptr)
				{
					FString AccessToken = JsonObject->GetStringField("access_token");
					FString IdToken= JsonObject->GetStringField("id_token");
					FString RefreshToken= JsonObject->GetStringField("refresh_token");

					RAGS->SetCognitoToken(AccessToken,IdToken,RefreshToken);

					TSharedRef<IHttpRequest> GetPlayerDataRequest = HttpModule->CreateRequest();
					GetPlayerDataRequest->OnProcessRequestComplete().BindUObject(this, &UMainMenuWidget::OnGetPlayerDataResponseReceived);
					GetPlayerDataRequest->SetURL(ApiUrl + "/getplayerdata");
					GetPlayerDataRequest->SetVerb("GET");
					GetPlayerDataRequest->SetHeader("Content-Type", "application/json");
					GetPlayerDataRequest->SetHeader("Authorization", AccessToken);
					GetPlayerDataRequest->ProcessRequest();
				}
			}
			else
			{
				UE_LOG(LogTemp,Error,TEXT("Error"));
			}
		}
	}
}

void UMainMenuWidget::OnClickedJoin()
{
	UE_LOG(LogTemp,Warning,TEXT("OnClickedJoin"));

	AMainMenuHUD* hud = Cast<AMainMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if(hud != nullptr)
	{
		hud->ShowLobbies();
	}
}

void UMainMenuWidget::OnClickedInventory()
{
	UZGameInstance* ZGameInstance = Cast<UZGameInstance>(GetGameInstance());
	if(ZGameInstance!= nullptr)
	{
		//route값 전달 index
		//ZGameInstance->ShowLoadingWidgetAfterLoadLevel("CityPark");
		ZGameInstance->LoadingAfterLoadLevel(ELevelName::ETrack_Cycling_Cinematic);
		
	}
}

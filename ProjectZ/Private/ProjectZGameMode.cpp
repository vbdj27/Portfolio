// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectZGameMode.h"
#include "ZBicycleCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ZPlayerController.h"
#include "ZGameInstance.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "ZInGameHUD.h"
#include "ZPlayerState.h"


AProjectZGameMode::AProjectZGameMode()
{
	HUDClass = AZInGameHUD::StaticClass();
}

//로그인시 캐릭터 스폰 및 소유권 할당
void AProjectZGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AZPlayerController* LocalPlayerController = Cast<AZPlayerController>(NewPlayer);
	
	SpawnCharacter(LocalPlayerController);

	AZPlayerState* ZPlayerState = Cast<AZPlayerState>(NewPlayer->PlayerState);
	if(ZPlayerState)
	{
		FString UserName = ZPlayerState->GetDisplayUserData().PlayerName;
		FString Country = GetEnumerationToString<ECountry>(ZPlayerState->GetDisplayUserData().Country);
		
		UE_LOG(LogTemp,Warning,TEXT("Server DP Data UserName: %s, Contry: %s"), *UserName, *Country);

		if(UserName.IsEmpty())
		{
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindUObject(this, &AProjectZGameMode::PlayerStateReTest, NewPlayer);

			GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.0f, false);
		}
	}
}

void AProjectZGameMode::PlayerStateReTest(APlayerController* PC)
{
	AZPlayerState* ZPlayerState = Cast<AZPlayerState>(PC->PlayerState);
	if(ZPlayerState)
	{
		FString UserName = ZPlayerState->GetDisplayUserData().PlayerName;
		FString Country = GetEnumerationToString<ECountry>(ZPlayerState->GetDisplayUserData().Country);
		
		UE_LOG(LogTemp,Warning,TEXT("Server DP Data UserName: %s, Contry: %s"), *UserName, *Country);
	}
}

void AProjectZGameMode::BeginPlay()
{
	Super::BeginPlay();

	
}

void AProjectZGameMode::PreInitializeComponents()
{

	Super::PreInitializeComponents();

}

FString AProjectZGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	SelectRoute = ExtractRoute(Options);
	UE_LOG(LogTemp,Error,TEXT("Options: %s"), *SelectRoute.ToString());
	
	return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

}


void AProjectZGameMode::SpawnCharacter(APlayerController* LocalPlayer)
{
	FVector Location = FVector(0.f, 0.f, 0.f);
	FRotator Rotation = FRotator(0.f, 0.f, 0.f);

	CharacterBPClass = LoadObject<UClass>(nullptr, TEXT("/Game/_BP/Characters/BP_ZBicycleCharacter.BP_ZBicycleCharacter_C"));

	for (APlayerStart* PlayerStart : TActorRange<APlayerStart>(GetWorld()))
	{
		if (PlayerStart->PlayerStartTag == SelectRoute)
		{
			Location = PlayerStart->GetActorLocation();
			Rotation = PlayerStart->GetActorRotation();

			if (CharacterBPClass)
			{
				SpawnedCharacter = GetWorld()->SpawnActor<AZBicycleCharacter>(CharacterBPClass, Location, Rotation);
				LocalPlayer->Possess(SpawnedCharacter);
				UE_LOG(LogTemp,Error,TEXT("Character Spawn Complete"));
				//UE_LOG(LogTemp, Log, TEXT("Spawned MyCharacter at (%f, %f, %f)"), Location.X, Location.Y, Location.Z);
						
			}
			return;
		}
	}

}

FName AProjectZGameMode::ExtractRoute(FString Options)
{
	TArray<FString> SplitOptions;
	Options.ParseIntoArray(SplitOptions, TEXT("?"), true);

	// 키(Key)와 값(Value)을 분리하여 저장할 맵
	TMap<FString, FString> KeyValueMap;

	// 각 분리된 요소를 순회하며 키와 값을 맵에 저장
	for (const FString& Option : SplitOptions)
	{
		FString Key, Value;
		if (Option.Split(TEXT("="), &Key, &Value))
		{
			KeyValueMap.Add(Key, Value);
		}
	}

	// "Data" 키의 값을 가져옴
	FString DataValue;
	KeyValueMap.RemoveAndCopyValue(TEXT("Route"), DataValue);

	return FName(*DataValue);
}

void AProjectZGameMode::ServerReceiveData_Implementation(int32 Data)
{
	UE_LOG(LogTemp,Error,TEXT("ServerReceiveData : %d"), Data);
}

bool AProjectZGameMode::ServerReceiveData_Validate(int32 Data)
{
	return true;
}

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectZGameMode.generated.h"

UCLASS(minimalapi)
class AProjectZGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectZGameMode();

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartingLoc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AZBicycleCharacter* SpawnedCharacter;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UClass* CharacterBPClass;

	// 다른 레벨에서 사용할 HUD 클래스들
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<class AHUD> MainMenu_HUD;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<class AHUD> InGame_HUD;

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void BeginPlay() override;
	virtual void PreInitializeComponents() override;
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal) override;
	void SpawnCharacter(APlayerController* LocalPlayer);

	FTimerHandle TimerHandle;
	
	void PlayerStateReTest(APlayerController* PC);


	// 서버에서 받은 데이터를 저장하기 위한 변수
	UPROPERTY(BlueprintReadWrite)
	FName SelectRoute;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReceiveData(int32 Data);

	FName ExtractRoute(FString Options);
};







// Fill out your copyright notice in the Description page of Project Settings.


#include "RouteSelectActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ZInGameHUD.h"
#include "ZBicycleCharacter.h"
#include "Widget/DirSelectWidget.h"
#include "Blueprint/UserWidget.h"
#include "Widget/InGameInfoWidget.h"

// Sets default values
ARouteSelectActor::ARouteSelectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	BoxCollision->SetupAttachment(RootComponent, NAME_None);
	BoxCollision->SetGenerateOverlapEvents(true);
	BoxCollision->SetCollisionProfileName(TEXT("Trigger"), false);

}

// Called when the game starts or when spawned
void ARouteSelectActor::BeginPlay()
{
	Super::BeginPlay();

	if(!HasAuthority())
	{
		ZInGameHUD = Cast<AZInGameHUD>(UGameplayStatics::GetPlayerController(GetWorld(),0)->GetHUD());

		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ARouteSelectActor::OnOverlap);
	}

}

// Called every frame
void ARouteSelectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARouteSelectActor::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 15.f, FColor::Red, *FString::Printf(TEXT("On Component Begin Overlap!!")));

	if (Cast<AZBicycleCharacter>(OtherActor) == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		//ZInGameHUD->InGameWidget->WBP_DirSelect->SetVisibility(ESlateVisibility::Visible);
		ZInGameHUD->IsLeft = IsLeft;
		ZInGameHUD->SplineName = SplineName;
		
		FTimerHandle WaitHandle;
		float WaitTime = 2.0f;
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{
				//ZInGameHUD->InGameWidget->WBP_DirSelect->SetVisibility(ESlateVisibility::Collapsed);
			}), WaitTime, false);

	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RouteSelectActor.generated.h"

UCLASS()
class PROJECTZ_API ARouteSelectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARouteSelectActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//OverlapComponents
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AZInGameHUD* ZInGameHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SplineName;

public:
	UFUNCTION(BlueprintCallable, Category = "Collision")
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};

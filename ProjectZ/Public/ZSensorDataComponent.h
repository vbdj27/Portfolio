// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZSensorDataComponent.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FSensorDataStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Watt;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		 float RiderWeight = 75.0;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		 float CycleWeight = 7.5;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		 float WheelArea = 0.509;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		 float DragCoefficient = 0.63;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	     float LossData = 2.0;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		 float WindSpeed = 0.0;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		float FrictionCoefficient = 0.005;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	    float RHO = 1.226;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		float CadenceRotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		float CadenceTime;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		float Slope;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTZ_API UZSensorDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UZSensorDataComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = "true"));
	class UPixelStreamingInput* ZPixelStreamingInputComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		FSensorDataStruct STR_SensorData;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	    float CurrentPower;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	    float CurrentSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		float CurrentCadence;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		FVector PreLoc;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	   class AZBicycleCharacter* Owner;

private:
	
	const FString Field_Power = "instantaneous_power"; //watt 필드 값

	const FString Field_Rev = "cumulative_crank_revs"; //Cadence 회전 값

	const FString Field_Time = "last_crank_event_time"; //Cadence 시간


public:
	UFUNCTION(BlueprintCallable)
		float CalcPowerMeter();

	UFUNCTION(BlueprintCallable)
		void ZOnInputEvent(const FString& Descriptor);

	UFUNCTION(BlueprintCallable)
		void CalcSpeed();

	UFUNCTION(BlueprintCallable)
	    float GetSlopeAngle();
	   

public:
	FString GetJsonFieldName(const FString& Val) const;

	
};


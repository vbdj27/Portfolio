// Fill out your copyright notice in the Description page of Project Settings.


#include "ZSensorDataComponent.h"
#include "PixelStreamingInputComponent.h"
#include "Misc/CString.h"
#include "Containers/UnrealString.h"
#include "ZBicycleCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"




// Sets default values for this component's properties
UZSensorDataComponent::UZSensorDataComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ZPixelStreamingInputComponent = CreateDefaultSubobject<UPixelStreamingInput>(TEXT("ZPixelStreamingInputComponent"));

}


// Called when the game starts
void UZSensorDataComponent::BeginPlay()
{
	Super::BeginPlay();

	if(!GetOwner()->HasAuthority())
	{
		Owner = Cast<AZBicycleCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	}

	ZPixelStreamingInputComponent->OnInputEvent.AddDynamic(this, &UZSensorDataComponent::ZOnInputEvent);
	
	ZPixelStreamingInputComponent->OnInputEvent.Broadcast("Descriptor");
}


// Called every frame
void UZSensorDataComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

//파워미터 계산 함수
float UZSensorDataComponent::CalcPowerMeter()
{
	float Gravity = 9.8067;
	float Weight = STR_SensorData.CycleWeight + STR_SensorData.RiderWeight;

	STR_SensorData.Slope = FMath::Tan((STR_SensorData.Slope * PI) / 180) * 100;

	double F_gravity = Gravity * sin(atan(STR_SensorData.Slope / 100)) * Weight;
	double F_rolling = Gravity * cos(atan(STR_SensorData.Slope / 100)) * Weight * STR_SensorData.FrictionCoefficient;

	double a = 0.5 * STR_SensorData.DragCoefficient * STR_SensorData.WheelArea * STR_SensorData.RHO;
	double b = STR_SensorData.WindSpeed * STR_SensorData.DragCoefficient * STR_SensorData.WheelArea * STR_SensorData.RHO;
	double c = F_gravity + F_rolling + (0.5 * STR_SensorData.DragCoefficient * STR_SensorData.WheelArea * STR_SensorData.RHO * STR_SensorData.WindSpeed * STR_SensorData.WindSpeed);
	double d = -1 * (1 - STR_SensorData.LossData / 100) * STR_SensorData.Watt;

	double Q = (3 * a * c - b * b) / (9 * a * a);
	double R = (3 * a * b * c - 27 * a * a * d - 2 * b * b * b) / (54 * a * a * a);

	double S = cbrt(R + sqrt((Q * Q * Q + R * R)));
	double T = cbrt(R - sqrt((Q * Q * Q + R * R)));

	double V = S + T - (b / 3 * a);

	bool isNaNCheck = FMath::IsNaN(V);
	if (!isNaNCheck)
	{
		CurrentPower = (float)V;
	}

	return CurrentPower;
}

//자전거 속도 계산 함수
void UZSensorDataComponent::CalcSpeed()
{
	CurrentCadence = (STR_SensorData.CadenceRotation / STR_SensorData.CadenceTime) * 60.0 * 10.0;
	//CurrentSpeed = CurrentPower * 100.0 / 5555.55;
	CurrentSpeed = CurrentPower * 100.0;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Speed: %f"), CurrentPower));
	
}

//픽셀스트리밍 입력 이벤트
void UZSensorDataComponent::ZOnInputEvent(const FString& Descriptor)
{
	if(!GetOwner()->HasAuthority())
	{ 
	UE_LOG(LogTemp, Warning, TEXT("Call BindingFunction!!"));

	 FString tempPower, tempRev, tempTime;
	 bool Success;
	
    //픽셀스트리밍 Json 데이터를 필드네임으로 가져옴
    ZPixelStreamingInputComponent->GetJsonStringValue(Descriptor, GetJsonFieldName(Field_Power), tempPower, Success);
	STR_SensorData.Watt = FCString::Atoi(*tempPower.TrimQuotes());

	ZPixelStreamingInputComponent->GetJsonStringValue(Descriptor, GetJsonFieldName(Field_Rev), tempRev, Success);
	STR_SensorData.CadenceRotation = FCString::Atof(*tempRev.TrimQuotes());

	ZPixelStreamingInputComponent->GetJsonStringValue(Descriptor, GetJsonFieldName(Field_Time), tempTime, Success);
	STR_SensorData.CadenceTime = FCString::Atof(*tempTime.TrimQuotes());

	GetSlopeAngle();
    CalcPowerMeter();
	CalcSpeed();
	Owner->SetMaxWalkSpeedToServer(CurrentSpeed);

	}
}

FString UZSensorDataComponent::GetJsonFieldName(const FString& Val) const
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, Val);
	return Val;
}

//경사 도출
float UZSensorDataComponent::GetSlopeAngle()
{

	STR_SensorData.Slope = UKismetMathLibrary::FindLookAtRotation(PreLoc, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation()).Pitch;

	PreLoc = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Slope: %f"), STR_SensorData.Slope));

   return STR_SensorData.Slope;
}
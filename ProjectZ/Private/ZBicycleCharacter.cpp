// Fill out your copyright notice in the Description page of Project Settings.


#include "ZBicycleCharacter.h"
#include "ProjectZCharacter.h"
#include "ZGameInstance.h"
#include "ZSplineMovementComponent.h"
#include "ZPlayerController.h"
#include "ZBikeAnimInstance.h"
#include "ZCyclistAnimInstance.h"

#include "EngineUtils.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ProjectZGameMode.h"
#include "ZInGameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "Widget/InGameInfoWidget.h"
#include "Widget/StatusWidget.h"


AZBicycleCharacter::AZBicycleCharacter()
{
	USkeletalMeshComponent* CharMesh = GetMesh();

	Frame = CharMesh;

	Wheel = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WheelDummy"));
	Wheel->SetupAttachment(CharMesh);

	Face = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FaceDummy"));
	Face->SetupAttachment(CharMesh);

	Headgear = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadgearDummy"));
	Headgear->SetupAttachment(CharMesh);

	Glasses = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GlassesDummy"));
	Glasses->SetupAttachment(CharMesh);

	Gloves = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GlovesDummy"));
	Gloves->SetupAttachment(CharMesh);

	Upper = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("UpperDummy"));
	Upper->SetupAttachment(CharMesh);

	Lower = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LowerDummy"));
	Lower->SetupAttachment(CharMesh);

	Shoes = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShoesDummy"));
	Shoes->SetupAttachment(CharMesh);

	check(Frame);
	check(Wheel);
	check(Face);
	check(Headgear);
	check(Glasses);
	check(Gloves);
	check(Upper);
	check(Lower);
	check(Shoes);


	testChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("testChildActor"));
	testChildActor->SetupAttachment(RootComponent);

	// first view camera
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoomFirst = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoomFirst"));
	CameraBoomFirst->SetupAttachment(RootComponent);
	// Create a follow camera
	FirstViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstViewCamera"));
	FirstViewCamera->SetupAttachment(CameraBoomFirst);


	// third view camera
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoomThird = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoomThird"));
	CameraBoomThird->SetupAttachment(Frame);
	CameraBoomThird->bEnableCameraRotationLag = true;
	CameraBoomThird->CameraRotationLagSpeed = 5;
	// Create a follow camera
	ThirdViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdViewCamera"));
	ThirdViewCamera->SetupAttachment(CameraBoomThird);

	// 
	MeshAttractor = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeshAttractor"));
	MeshAttractor->SetupAttachment(RootComponent);

	AvoidanceBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AvoidanceBox"));
	AvoidanceBox->SetupAttachment(MeshAttractor);
	//AvoidanceBox->OnComponentBeginOverlap.AddDynamic(this, &AZBicycleCharacter::OnOverlapBegin);
}

void AZBicycleCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	DOREPLIFETIME(AZBicycleCharacter, CurrentBikeSpeed);
	DOREPLIFETIME(AZBicycleCharacter, CurrenFrameRelativeLocation);
}

void AZBicycleCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	FrameMaterialInstance = Frame->CreateDynamicMaterialInstance(0);
	FrameBackwardMaterialInstance = Frame->CreateDynamicMaterialInstance(1);
	WheelMaterialInstance = Wheel->CreateDynamicMaterialInstance(0);
	FaceMaterialInstance = Face->CreateDynamicMaterialInstance(0);
	GlovesMaterialInstance = Gloves->CreateDynamicMaterialInstance(0);
	HeadgearMaterialInstance = Headgear->CreateDynamicMaterialInstance(0);
	GlassesMaterialInstance = Glasses->CreateDynamicMaterialInstance(1);
	UpperMaterialInstance = Upper->CreateDynamicMaterialInstance(0);
	LowerMaterialInstance = Lower->CreateDynamicMaterialInstance(0);
	ShoesMaterialInstance = Shoes->CreateDynamicMaterialInstance(0);


	// Anim
	BikeAnim = Cast<UZBikeAnimInstance>(GetMesh()->GetAnimInstance());
	CyclistAnim = Cast<UZCyclistAnimInstance>(Face->GetAnimInstance());
}

void AZBicycleCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitAllParts();

	FInputActionValue temp;
	ThirdPersonView(temp);

	// Initalize Rep Var
	CurrenFrameRelativeLocation = Frame->GetRelativeLocation();
	CurrentBikeSpeed = GetCharacterMovement()->MaxWalkSpeed;

	
	if(!HasAuthority())
	{
		UZGameInstance* ZGameInstance = Cast<UZGameInstance>(GetGameInstance());
		if(ZGameInstance)
		{
			ServerSendBroadCastRoute(ZGameInstance->StartingPoint);
		}
	}

	//temp local
	SplineMovementComp = Cast<UZSplineMovementComponent>(GetComponentByClass(UZSplineMovementComponent::StaticClass()));
	
	if (SplineMovementComp)
	{
		UZGameInstance* ZGameInstance = Cast<UZGameInstance>(GetGameInstance());
		if(ZGameInstance)
		{
			SplineMovementComp->SetInitSplineComponentWithTag(ZGameInstance->StartingPoint);
		}
	}
	
}


void AZBicycleCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CalculateBycleSpeed(DeltaSeconds);
}

void AZBicycleCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	UE_LOG(LogTemp, Warning, TEXT("PlayerState 복제가능시점"));

	UZGameInstance* ZGameInstance = Cast<UZGameInstance>(GetGameInstance());
	if (ZGameInstance)
	{
		AZPlayerController* ZPlayerController = Cast<AZPlayerController>(GetController());
		if (ZPlayerController)
		{
			FDisplayUserData UserData;
			UserData.PlayerName = ZGameInstance->GetAvatarInfo().UserName;
			UserData.Country = ZGameInstance->GetAvatarInfo().Country;


			ZPlayerController->ServerSendDisplayUserData(UserData);
		}
	}
}

void AZBicycleCharacter::GetIngameHUD()
{

	AZInGameHUD* InGameHUD = Cast<AZInGameHUD>(UGameplayStatics::GetPlayerController(this,0)->GetHUD());
	if(InGameHUD)
	{
		UInGameInfoWidget* InGameInfoWidget = Cast<UInGameInfoWidget>(InGameHUD->InGameWidget);
		if(InGameInfoWidget)
		{
			InGameInfoWidget->StatusWidget->UpdateBicycleStatus();
		}
	}
}

void AZBicycleCharacter::ReqSetCharacterLocation_Implementation(FVector Loc, FRotator Rot)
{
	
	SetActorLocation(Loc, false, 0, ETeleportType::TeleportPhysics);
	SetActorRotation(Rot, ETeleportType::TeleportPhysics);
}

void AZBicycleCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Default Camera View
		EnhancedInputComponent->BindAction(DefaultViewAction, ETriggerEvent::Completed, this,
		                                   &AZBicycleCharacter::DefaultCameraView);

		//Third Person Camera
		EnhancedInputComponent->BindAction(ThirdViewAction, ETriggerEvent::Completed, this,
		                                   &AZBicycleCharacter::ThirdPersonView);

		//First Person Camera
		EnhancedInputComponent->BindAction(FirstViewAction, ETriggerEvent::Completed, this,
		                                   &AZBicycleCharacter::FirstPersonView);


		// max walk speed
		EnhancedInputComponent->BindAction(AddSpeedAction, ETriggerEvent::Completed, this,
		                                   &AZBicycleCharacter::LocalAddSpeed);
		EnhancedInputComponent->BindAction(SubSpeedAction, ETriggerEvent::Completed, this,
		                                   &AZBicycleCharacter::LocalSubSpeed);
	}
}


void AZBicycleCharacter::DefaultCameraView(const FInputActionValue& Value)
{
	FirstViewCamera->Deactivate();
	ThirdViewCamera->Deactivate();

	FollowCamera->Activate();

	//
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoomThird->bUsePawnControlRotation = false;
}

void AZBicycleCharacter::ThirdPersonView(const FInputActionValue& Value)
{
	FollowCamera->Deactivate();
	FirstViewCamera->Deactivate();

	ThirdViewCamera->Activate();

	//
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoomThird->bUsePawnControlRotation = false;
}

void AZBicycleCharacter::FirstPersonView(const FInputActionValue& Value)
{
	FollowCamera->Deactivate();
	ThirdViewCamera->Deactivate();

	FirstViewCamera->Activate();

	//
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoomThird->bUsePawnControlRotation = false;
}

void AZBicycleCharacter::AddMaxWalkSpeedToServer_Implementation()
{
	//AddMaxSpeedToMulticast();

	auto CurrentMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	CurrentMaxWalkSpeed += 100;
	SetMaxWalkSpeedMulticast(CurrentMaxWalkSpeed);

	// 	if (IsLocallyControlled())
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("%.f"), CurrentMaxWalkSpeed));
}

void AZBicycleCharacter::SubMaxWalkSpeedToServer_Implementation()
{
	//SubMaxSpeedToMulticast();

	auto CurrentMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	CurrentMaxWalkSpeed -= 100;
	SetMaxWalkSpeedMulticast(CurrentMaxWalkSpeed);

	// 	if (IsLocallyControlled())
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("%.f"), CurrentMaxWalkSpeed));
}

void AZBicycleCharacter::LocalAddSpeed()
{
	auto CurrentMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	CurrentMaxWalkSpeed += 100;
	GetCharacterMovement()->MaxWalkSpeed = CurrentMaxWalkSpeed;

	// 	if (IsLocallyControlled())
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("%.f"), CurrentMaxWalkSpeed));
}

void AZBicycleCharacter::LocalSubSpeed()
{
	auto CurrentMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	CurrentMaxWalkSpeed -= 100;
	GetCharacterMovement()->MaxWalkSpeed = CurrentMaxWalkSpeed;

	// 	if (IsLocallyControlled())
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("%.f"), CurrentMaxWalkSpeed));
}


void AZBicycleCharacter::SetMaxWalkSpeedToServer_Implementation(float CurrentSpeed)
{
	SetMaxWalkSpeedMulticast(CurrentSpeed);
}


void AZBicycleCharacter::SetMaxWalkSpeedMulticast_Implementation(float CurrenSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = CurrenSpeed;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
	                                 FString::Printf(
		                                 TEXT("max walk speed: %.f"), GetCharacterMovement()->MaxWalkSpeed));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("CurrentSpeed: %.f"), CurrenSpeed));
}

void AZBicycleCharacter::AddOverlappedBikeCountMulticast_Implementation()
{
	OverlappedBikeCount++;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue,
	                                 FString::Printf(TEXT("multicast AddOverlappedBikeCountMulticast")));
}

void AZBicycleCharacter::SubOverlappedBikeCountMulticast_Implementation()
{
	OverlappedBikeCount--;

	if (OverlappedBikeCount <= 0)
		OverlappedBikeCount = 0;


	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
	                                 FString::Printf(TEXT("multicast SubOverlappedBikeCountMulticast")));
}

void AZBicycleCharacter::ServerSendBroadCastRoute_Implementation(FName Route)
{
	MulticastBroadcastRoute(Route);
}

bool AZBicycleCharacter::ServerSendBroadCastRoute_Validate(FName Route)
{
	return true;
}

void AZBicycleCharacter::MulticastBroadcastRoute_Implementation(FName Route)
{
	// Set SplineMovementComp
	SplineMovementComp = Cast<UZSplineMovementComponent>(GetComponentByClass(UZSplineMovementComponent::StaticClass()));
	
	if (SplineMovementComp)
	{
		SplineMovementComp->SetInitSplineComponentWithTag(Route);
	}
}

void AZBicycleCharacter::InitAllParts()
{
	UZGameInstance* ZGameInstance = Cast<UZGameInstance>(GetGameInstance());
	if (ZGameInstance)
	{
		FPlayerEquipmentInfo PlayerEquipmentInfo = ZGameInstance->GetAvatarInfo().PlayerEquipmentInfo;
		int32 FrameIndex = PlayerEquipmentInfo.FrameIndex;
		int32 WheelIndex = PlayerEquipmentInfo.WheelIndex;
		int32 HeadgearIndex = PlayerEquipmentInfo.HeadgearIndex;
		int32 GlassesIndex = PlayerEquipmentInfo.GlassesIndex;
		int32 JerseyIndex = PlayerEquipmentInfo.JerseyIndex;
		int32 GlovesIndex = PlayerEquipmentInfo.GlovesIndex;
		int32 SocksIndex = PlayerEquipmentInfo.SocksIndex;
		int32 ShoesIndex = PlayerEquipmentInfo.ShoesIndex;

		FrameIndex %= 10;
		WheelIndex %= 10;
		HeadgearIndex %= 10;
		GlassesIndex %= 10;
		JerseyIndex %= 10;
		GlovesIndex %= 10;
		SocksIndex %= 10;
		ShoesIndex %= 10;

		switch (FrameIndex)
		{
		default:
			break;
		case 0:
			FrameMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureFrameForwardG);
			FrameBackwardMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureFrameBackwardG);
			break;
		case 1:
			FrameMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureFrameForwardB);
			FrameBackwardMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureFrameBackwardB);
			break;
		case 2:
			FrameMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureFrameForwardR);
			FrameBackwardMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureFrameBackwardR);
			break;
		}

		switch (WheelIndex)
		{
		default:
			break;
		case 0:
			WheelMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureWheelBK);
			break;
		case 1:
			WheelMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureWheelG);
			break;
		case 2:
			WheelMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureWheelB);
			break;
		case 3:
			WheelMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureWheelR);
			break;
		}

		switch (HeadgearIndex)
		{
		default:
			break;
		case 0:
			HeadgearMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureGreen);
			break;
		case 1:
			HeadgearMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureBlue);
			break;
		case 2:
			HeadgearMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureRed);
			break;
		}

		switch (GlassesIndex)
		{
		default:
			break;
		case 0:
			GlassesMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureGlassesGreen);
			break;
		case 1:
			GlassesMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureGlassesBlue);
			break;
		case 2:
			GlassesMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureGlassesRed);
			break;
		}

		switch (JerseyIndex)
		{
		default:
			break;
		case 0:
			UpperMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureGreen);
			LowerMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureGreen);
			break;
		case 1:
			UpperMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureBlue);
			LowerMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureBlue);
			break;
		case 2:
			UpperMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureRed);
			LowerMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureRed);
			break;
		}

		switch (GlovesIndex)
		{
		default:
			break;
		case 0:
			GlovesMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureGreen);
			break;
		case 1:
			GlovesMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureBlue);
			break;
		case 2:
			GlovesMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureRed);
			break;
		}

		switch (ShoesIndex)
		{
		default:
			break;
		case 0:
			ShoesMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureGreen);
			break;
		case 1:
			ShoesMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureWhite);
			break;
		}
	}
}


// collision

void AZBicycleCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                        const FHitResult& SweepResult)
{
	if (OtherActor == this)
		return;


	if (GetLocalRole() == ROLE_Authority)
		AddOverlappedBikeCountMulticast();

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("beginOverlap")));
}

void AZBicycleCharacter::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                      class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == this)
		return;

	if (GetLocalRole() == ROLE_Authority)
		SubOverlappedBikeCountMulticast();

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("endOverlap")));
}

void AZBicycleCharacter::SetCurrentSplineComponent(const class AActor* SplineActor)
{
	// 	auto SplineComp = Cast<UZSplineMovementComponent>(GetComponentByClass(UZSplineMovementComponent::StaticClass()));
	if (SplineMovementComp)
		SplineMovementComp->SetCurrentSplineComponent(SplineActor);
	else
		UE_LOG(LogTemp, Warning, TEXT(" ZBicycleCharacter:: There is no UZSplineMovementComponent "));
}


void AZBicycleCharacter::OnRep_CurrentBikeSpeed()
{
	//float CharMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	CyclistAnim->MaxWalkSpeed = CurrentBikeSpeed;
	BikeAnim->MaxWalkSpeed = CurrentBikeSpeed;
}

void AZBicycleCharacter::OnReop_CurrenFrameRelativeLocation()
{
	Frame->SetRelativeLocation(CurrenFrameRelativeLocation);

	//UE_LOG(LogTemp, Warning, TEXT(" ZBicycleCharacter:: REp CurrenFrameRelativeLocation "));
}

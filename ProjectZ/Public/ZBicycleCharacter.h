// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "ProjectZCharacter.h"
#include "Components/CapsuleComponent.h"
#include "ZBicycleCharacter.generated.h"


//DECLARE_MULTICAST_DELEGATE_OneParam(FTestMultiDelegate, int32 param);


/**
 * 
 */
UCLASS()
class PROJECTZ_API AZBicycleCharacter : public AProjectZCharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parts, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Frame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parts, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Wheel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parts, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Face;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parts, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Gloves;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parts, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Headgear;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parts, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Glasses;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parts, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Upper;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parts, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Lower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parts, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Shoes;


	UPROPERTY()
	class UMaterialInstanceDynamic* FrameMaterialInstance;
	UPROPERTY()
	class UMaterialInstanceDynamic* FrameBackwardMaterialInstance;
	UPROPERTY()
	class UMaterialInstanceDynamic* WheelMaterialInstance;
	UPROPERTY()
	class UMaterialInstanceDynamic* FaceMaterialInstance;
	UPROPERTY()
	class UMaterialInstanceDynamic* GlovesMaterialInstance;
	UPROPERTY()
	class UMaterialInstanceDynamic* HeadgearMaterialInstance;
	UPROPERTY()
	class UMaterialInstanceDynamic* GlassesMaterialInstance;
	UPROPERTY()
	class UMaterialInstanceDynamic* UpperMaterialInstance;
	UPROPERTY()
	class UMaterialInstanceDynamic* LowerMaterialInstance;
	UPROPERTY()
	class UMaterialInstanceDynamic* ShoesMaterialInstance;

	friend class ZSplineMovementComponent;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parts)
	class UChildActorComponent* testChildActor;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	class UBoxComponent* AvoidanceBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	class UCapsuleComponent* MeshAttractor;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoomFirst;

	/** FirstView camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstViewCamera;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoomThird;

	/** FirstView camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* ThirdViewCamera;


	/** Default Camera View Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DefaultViewAction;

	/** Third Person Camera View Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ThirdViewAction;

	/** First Person Camera View Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FirstViewAction;


	/** Add Max Walk Speed Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AddSpeedAction;
	/** Sub Max Walk Speed Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SubSpeedAction;

	
	// 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// 	class UZSplineMovementComponent* ZSplineMovementComponent;
public:
	AZBicycleCharacter();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	void CalculateBycleSpeed(float DeltaSeconds);

	virtual void Tick(float DeltaSeconds) override;

	

	virtual void OnRep_PlayerState() override;

	void GetIngameHUD();

	//누적거리
	UPROPERTY()
	float TotalDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SpawnPoint;
	
	//texture
	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureGreen;

	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureBlue;

	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureRed;

	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureWhite;

	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureGlassesGreen;
	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureGlassesBlue;
	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureGlassesRed;

	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureFrameForwardR;
	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureFrameForwardG;
	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureFrameForwardB;
	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureFrameBackwardR;
	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureFrameBackwardG;
	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureFrameBackwardB;

	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureWheelBK;
	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureWheelR;
	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureWheelG;
	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureWheelB;

protected:
	/** Called for looking input */
	void DefaultCameraView(const FInputActionValue& Value);
	void ThirdPersonView(const FInputActionValue& Value);
	void FirstPersonView(const FInputActionValue& Value);

public:
	/** Called for control Character movement->MaxWalkSpeed */
	/** Server RPC*/
	UFUNCTION(Server, Reliable)
	void AddMaxWalkSpeedToServer();
	void AddMaxWalkSpeedToServer_Implementation();

	UFUNCTION(Server, Reliable)
	void SubMaxWalkSpeedToServer();
	void SubMaxWalkSpeedToServer_Implementation();

	UFUNCTION()
	void LocalAddSpeed();

	UFUNCTION()
	void LocalSubSpeed();


	UFUNCTION(Server, Reliable)
	void SetMaxWalkSpeedToServer(float CurrentSpeed);
	void SetMaxWalkSpeedToServer_Implementation(float CurrentSpeed);

	/** Called for control Character movement->MaxWalkSpeed */
	/** Multicast RPC*/

	UFUNCTION(NetMulticast, Reliable)
	void SetMaxWalkSpeedMulticast(float CurrenSpeed);
	void SetMaxWalkSpeedMulticast_Implementation(float CurrenSpeed);

	UFUNCTION(NetMulticast, Reliable)
	void AddOverlappedBikeCountMulticast();
	void AddOverlappedBikeCountMulticast_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void SubOverlappedBikeCountMulticast();
	void SubOverlappedBikeCountMulticast_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSendBroadCastRoute(FName Route);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastBroadcastRoute(FName Route);


	void InitAllParts();

	UFUNCTION(Server, Reliable)
	void ReqSetCharacterLocation(FVector Loc, FRotator Rot);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	float speed;
	FVector MoveDirection;
	FRotator MoveRotation;
	bool bRVOActivation;

	TArray<class AProjectZCharacter*> OverlappedCharacterArray;

	class UZSplineMovementComponent* SplineMovementComp;

public:
	// RepNotify
	UPROPERTY(ReplicatedUsing = OnRep_CurrentBikeSpeed)
	float CurrentBikeSpeed;
	UFUNCTION()
	void OnRep_CurrentBikeSpeed();


	UPROPERTY(ReplicatedUsing = OnReop_CurrenFrameRelativeLocation)
	FVector CurrenFrameRelativeLocation;
	UFUNCTION()
	void OnReop_CurrenFrameRelativeLocation();

public:
	UFUNCTION()
	FVector GetMeshAttractionLocation() { return MeshAttractor->GetComponentLocation(); }

	UFUNCTION()
	class UCapsuleComponent* GetMeshAttraction() { return MeshAttractor; };

	UFUNCTION()
	class USkeletalMeshComponent* GetFrame() { return Frame; };

	UFUNCTION(BlueprintCallable)
	class UZSplineMovementComponent* GetSplineMovementComponent() { return SplineMovementComp; };

	//Anim
public:
	UPROPERTY()
	class UZBikeAnimInstance* BikeAnim;
	UPROPERTY()
	class UZCyclistAnimInstance* CyclistAnim;

	// collision
public:
	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                  class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION(BlueprintGetter)
	int GetOverlappedBikeCount() const { return OverlappedBikeCount; };


	UFUNCTION(BlueprintCallable)
	void SetCurrentSplineComponent(const class AActor* SplineActor);

private:
	UPROPERTY(BlueprintGetter = GetOverlappedBikeCount)
	int OverlappedBikeCount = 0;
};

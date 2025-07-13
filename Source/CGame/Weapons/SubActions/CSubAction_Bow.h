// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "Components/TimelineComponent.h"
#include "CSubAction_Bow.generated.h"

USTRUCT()
struct FAimData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float TargetArmLength = 100;

	UPROPERTY(EditAnywhere)
	FVector SocketOffset = FVector(0, 30, 10);

	UPROPERTY(EditAnywhere)
	bool bEnableCameraLag;

	UPROPERTY(EditAnywhere) //카메라 위치 보정값 , 스프링암의 소켓 오프셋을 조정함
	FVector CameraLocation;
};


UCLASS(Blueprintable)
class CGAME_API UCSubAction_Bow : public UCSubAction
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Aiming")
	UCurveVector* Curve;
	
	UPROPERTY(EditAnywhere,Category = "Aiming")
	FAimData AimData;

	UPROPERTY(EditAnywhere,Category = "Aiming")
	float AimingSpeed = 200.0f ;

private:
	float* Bending;
	
public:
	UCSubAction_Bow();
	void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction) override;
	void Tick(float InDeltaTime) override;

public:
	void Pressed() override;
	void Released() override;

private:
	FAimData OriginData;
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;
	FTimeline Timeline;

private:
	UFUNCTION()
	void OnAiming(FVector Output);
};

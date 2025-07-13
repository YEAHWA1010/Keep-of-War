// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SubActions/CSubAction_Bow.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CStateComponent.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveVector.h"
#include "Weapons/Attachments/CAttachment_Bow.h"

UCSubAction_Bow::UCSubAction_Bow()
{	
	CHelpers::GetAssetDynamic<UCurveVector>(&Curve,"CurveVector'/Game/Characters/Weapons/ElvenBow/Curve_Aiming.Curve_Aiming'");
}

void UCSubAction_Bow::BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment,
	class UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	SpringArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
	Camera = CHelpers::GetComponent<UCameraComponent>(InOwner);

	FOnTimelineVector timeline;
	timeline.BindUFunction(this,"OnAiming");
	Timeline.AddInterpVector(Curve,timeline); //Curve를 실행하면서 매프레임마다 현재 시간에 맞는 값들을 리턴해줌 , 델리게이션 하나를 부름

	Timeline.SetPlayRate(AimingSpeed);
	ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);
	if (!!bow)
	{
		Bending = bow->GetAnimInstance_Bending();
	}
	
}

void UCSubAction_Bow::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);
	Timeline.TickTimeline(InDeltaTime);
}

void UCSubAction_Bow::Pressed()
{
	CheckNull(SpringArm);
	CheckNull(Camera);
	
	Super::Pressed();
	
	OriginData.TargetArmLength = SpringArm->TargetArmLength;
	OriginData.SocketOffset = SpringArm->SocketOffset;
	OriginData.bEnableCameraLag = SpringArm->bEnableCameraLag;
	OriginData.CameraLocation = Camera->GetRelativeLocation();

	SpringArm->TargetArmLength = AimData.TargetArmLength;
	SpringArm->SocketOffset = AimData.SocketOffset;
	SpringArm->bEnableCameraLag = AimData.bEnableCameraLag;
	Camera->SetRelativeLocation(AimData.CameraLocation);

	//시야각 45도
	//Camera->FieldOfView = 45;

	Timeline.PlayFromStart();
}

void UCSubAction_Bow::Released()
{
	CheckNull(SpringArm);
	CheckNull(Camera);
	
	Super::Released();

	State->OffSubActionMode();

	SpringArm->TargetArmLength = OriginData.TargetArmLength;
	SpringArm->SocketOffset = OriginData.SocketOffset;
	SpringArm->bEnableCameraLag = OriginData.bEnableCameraLag;
	Camera->SetRelativeLocation(OriginData.CameraLocation);

	//Camera->FieldOfView = 90;

	Timeline.ReverseFromEnd();
}

void UCSubAction_Bow::OnAiming(FVector Output)
{
	Camera->FieldOfView = Output.X;
	if (!!Bending)
	{
		*Bending =  Output.Y;
	}
}

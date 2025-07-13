// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/DoActions/CDoAction_Warp.h"
#include "Weapons/CAttachment.h"
#include "GameFramework/PlayerController.h"
#include "Global.h"
#include "Components/CStateComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/CAIBehaviorComponent.h"


UCDoAction_Warp::UCDoAction_Warp()
{
	
}

void UCDoAction_Warp::BeginPlay(class ACharacter* InOwner, ACAttachment* InAttachment, UCEquipment* InEquipment,const TArray<FDoActionData>& InDoActionDatas,
	const TArray<FHitData>& InHitDatas)
{
	Super::BeginPlay(InOwner,InAttachment,InEquipment,InDoActionDatas, InHitDatas);

	Decal = CHelpers::GetComponent<UDecalComponent>(InAttachment);
	PlayerController = InOwner->GetController<APlayerController>();

	Behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(InOwner);
}

void UCDoAction_Warp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Location = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;

	if (GetCursorLocationAndRotation(Location, Rotation) == false)
	{
		Decal->SetVisibility(false);

		return;
	}

	Decal->SetVisibility(true);
	
	//ImpactPoint는 충돌 지점
	Decal->SetWorldLocation(Location);
	Decal->SetWorldRotation(Rotation); //world를 쓰는 이유는 리턴되는게 월드값이어서
	//노멀은 충돌한 애의 수직 => 그 방향으로 회전
	//CLog::Print(hitresult.ImpactPoint,9999);
}

void UCDoAction_Warp::DoAction()
{
	CheckFalse(State->IsIdleMode());
	CheckFalse(DoActionDatas.Num() > 0);

	Super::DoAction();

	FRotator Rotation;
	//이동할 위치를 구함
	if (GetCursorLocationAndRotation(MoveToLocation, Rotation))
	{
		//반만 올림 , 중심점이 발에 있으니까
		float Height = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		MoveToLocation.Z += Height;

		float Yaw = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(),MoveToLocation).Yaw;
		OwnerCharacter->SetActorRotation(FRotator(0,Yaw,0));
	}

	DoActionDatas[0].DoAction(OwnerCharacter);
}

void UCDoAction_Warp::Begin_DoAction()
{
	Super::Begin_DoAction();

	if (!!PlayerController)
	{
		OwnerCharacter->SetActorLocation(MoveToLocation);
		MoveToLocation = FVector::ZeroVector;

		return;
	}

	CheckNull(Behavior);
	OwnerCharacter->SetActorLocation(Behavior->GetEqsLocation());
}

bool UCDoAction_Warp::GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation)
{
	CheckNullResult(PlayerController,false);

	FHitResult hitresult;
	//Channel 채널1번은 기본적으로 다 지정
	PlayerController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1,false,hitresult);
	CheckFalseResult(hitresult.bBlockingHit,false);

	OutLocation = hitresult.Location;
	OutRotation = hitresult.ImpactNormal.Rotation();
	
	return true;
}


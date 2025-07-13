// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/DoActions/CDoAction_Combo.h"
#include "Global.h"
#include "Chaos/Utilities.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "GameFramework/PlayerController.h"

#include "Engine/DamageEvents.h"

void UCDoAction_Combo::DoAction()
{
	CheckTrue(DoActionDatas.Num() < 1); //데이터가 없다 => 공격 수행 못함
	if (bEnable)
	{
		bEnable = false;
		bExist = true;

		return;
	}

	CheckFalse(State->IsIdleMode());

	Super::DoAction();
	DoActionDatas[Index].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::Begin_DoAction()
{
	Super::Begin_DoAction();
	CheckFalse(bExist);

	bExist = false;
	
	//다음 플레이전 현재꺼 삭제
	DoActionDatas[Index].Destroy_GhostTrail();
	DoActionDatas[++Index].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::End_DoAction()
{
	Super::End_DoAction();

	//다음 플레이전 현재꺼 삭제
	DoActionDatas[Index].Destroy_GhostTrail();
	Index = 0;
}

void UCDoAction_Combo::OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser,
	class ACharacter* InOther)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther);

	//공격 받는 애가 없으면 수행 x
	CheckNull(InOther);
	
	// FActionDamageEvent e;
	// e.HitData = &HitDatas[Index];
	//
	// InOther -> TakeDamage(HitDatas[Index].Power,e,InAttacker->GetController(),InAttackCauser);

	//원래꺼에 있다면 안하고 없으면 한다
	for (ACharacter* hitted : Hitted)
	{
		CheckTrue(hitted == InOther);
	}

	Hitted.AddUnique(InOther);
	
	CheckTrue(HitDatas.Num() - 1 < Index);
	HitDatas[Index].SendDamage(InAttacker,InAttackCauser,InOther);
}

void UCDoAction_Combo::OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther)
{
	
}

void UCDoAction_Combo::OnAttachmentEndCollision()
{
	Super::OnAttachmentEndCollision();
	
	Hitted.Empty();

	float Angle = -2.0f;
	ACharacter* Candidate = nullptr;

	for (ACharacter* hitted : Hitted)
	{
		FVector Direction = hitted->GetActorLocation() - OwnerCharacter->GetActorLocation();
		Direction.GetSafeNormal2D().Normalize();

		FVector Forward = OwnerCharacter->GetActorForwardVector();

		//내적
		float Dot = FVector::DotProduct(Direction, Forward);
		if (Dot < 0.75f || Dot < Angle) //45도 보다 작으면 안함
		{
			continue;
		}
		//회전 대상
		Angle = Dot;
		Candidate = hitted;
	}

	if (!!Candidate)
	{
		FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(),Candidate->GetActorLocation());
		
		APlayerController* Controller = OwnerCharacter->GetController<APlayerController>();

		if (!!Controller) //적인 경우
		{
			FRotator ControlRotaiton = Controller->GetControlRotation();
			FRotator Target = FRotator(ControlRotaiton.Pitch, Rotator.Yaw, ControlRotaiton.Roll);
			
			Controller->SetControlRotation(Target);
		}
	}

	Hitted.Empty();
}

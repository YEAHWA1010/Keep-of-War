// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CDoAction.h"
#include "Global.h"
#include "CAttachment.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"

UCDoAction::UCDoAction()
{
	
}

void UCDoAction::BeginPlay( class ACharacter* InOwner, ACAttachment* InAttachment,UCEquipment* InEquipment,
	const TArray<FDoActionData>& InDoActionDatas, const TArray<FHitData>& InHitDatas)
{
	OwnerCharacter = InOwner;
	World = OwnerCharacter->GetWorld();
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);

	//DoActionDatas.
	//배열끼리 대입하면 무조건 복사 일어남 => 좋지 않은 방법으로 원래는 clone을 사용해야한다
	DoActionDatas = InDoActionDatas;
	HitDatas = InHitDatas;
}

void UCDoAction::DoAction()
{
	State->SetActionMode();
}

//노티파이를 통해
void UCDoAction::Begin_DoAction()
{
	bBeginAction = true;
}
//노티파이를 통해
void UCDoAction::End_DoAction()
{
	bBeginAction = false;
	bInAction = false;
	
	State->SetIdleMode();
	Movement-> Move();
}








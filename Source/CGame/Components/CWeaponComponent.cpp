// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CWeaponComponent.h"

#include "GeometryTypes.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Weapons/CWeaponAsset.h"
#include "Components/CStateComponent.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/SubActions/CSubAction_Bow.h"
#include "Weapons/CWeaponData.h"


class ACAttachment* UCWeaponComponent::GetAttachment()
{
	CheckTrueResult(IsUnarmedMode(),nullptr);
	CheckFalseResult(!!Datas[(int32)Type],nullptr);

	//현재 선택된 무기에 대한 attachment 리턴
	return Datas[(int32)Type]->GetAttachment();
}

class UCEquipment* UCWeaponComponent::GetEquipment()
{
	CheckTrueResult(IsUnarmedMode(),nullptr);
	CheckFalseResult(!!Datas[(int32)Type],nullptr);

	//현재 선택된 무기에 대한 attachment 리턴
	return Datas[(int32)Type]->GetEquipment();
}

class UCDoAction* UCWeaponComponent::GetDoAction()
{
	CheckTrueResult(IsUnarmedMode(),nullptr);
	CheckFalseResult(!!Datas[(int32)Type],nullptr);
	
	return Datas[(int32)Type]->GetDoAction();
}

class UCSubAction* UCWeaponComponent::GetSubAction()
{
	CheckTrueResult(IsUnarmedMode(),nullptr);
	CheckFalseResult(!!Datas[(int32)Type],nullptr);
	
	return Datas[(int32)Type]->GetSubAction();
}

// Sets default values for this component's properties
UCWeaponComponent::UCWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UCWeaponComponent::IsIdleMode()
{
	return CHelpers::GetComponent<UCStateComponent>(OwnerCharacter)->IsIdleMode();
}


// Called when the game starts
void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	
	for (int i = 0; i < (int32)EWeaponType::Max; i++)
	{
		//배열형태로 하면 무기번호에 맞게 해야함
		if (!!DataAssets[i])
		{
			//DataAssets[i]-> BeginPlay(OwnerCharacter);
			DataAssets[i]-> BeginPlay(OwnerCharacter,&Datas[i]);
		}
	}
}


// Called every frame
void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!!GetDoAction())
	{
		GetDoAction()->Tick(DeltaTime);
	}

	if (!!GetSubAction())
	{
		GetSubAction()->Tick(DeltaTime);
	}

	// ...
}

void UCWeaponComponent::SetUnarmedMode()
{
	GetEquipment()->UnEquip();
	
	ChangeType(EWeaponType::Max);
}

void UCWeaponComponent::SetSwordMode()
{
	CheckFalse(IsIdleMode());
	SetMode(EWeaponType::Sword);
}

void UCWeaponComponent::SetHammerMode()
{
	CheckFalse(IsIdleMode());
	SetMode(EWeaponType::Hammer);
}

void UCWeaponComponent::SetFistMode()
{
	CheckFalse(IsIdleMode());
	SetMode(EWeaponType::Fist);
}

void UCWeaponComponent::SetWarpMode()
{
	CheckFalse(IsIdleMode());
	SetMode(EWeaponType::Warp);
}

void UCWeaponComponent::SetBowMode()
{
	CheckFalse(IsIdleMode());
	SetMode(EWeaponType::Bow);
}

void UCWeaponComponent::SetMode(EWeaponType InType)
{
	if (Type == InType)
	{
		SetUnarmedMode(); //얘는 완전 해제
		return;
	}
	else if (IsUnarmedMode() == false)
	{
		//현재 무기에서 다른 무기로 바꾸기 위해 해제
		GetEquipment()->UnEquip();
	}
	if (!!Datas[(int32)InType])
	{
		//무기 장착
		Datas[(int32)InType]->GetEquipment()->Equip();

		ChangeType(InType); //타입 변경
		
	}
}


void UCWeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType PrevType = Type;
	Type = InType;

	if (OnWeaponTypeChanged.IsBound())
	{
		//멀티캐스트니까 브로드캐스트로 콜한다 
		OnWeaponTypeChanged.Broadcast(PrevType, InType);
	}
}

void UCWeaponComponent::DoAction()
{
	if (!!GetDoAction())
	{
		GetDoAction()->DoAction();
	}
}

void UCWeaponComponent::SubAction_Pressed()
{
	if (!!GetSubAction())
	{
		GetSubAction()->Pressed();
	}
}

void UCWeaponComponent::SubAction_Released()
{
	if (!!GetSubAction())
	{
		GetSubAction()->Released();
	}
}


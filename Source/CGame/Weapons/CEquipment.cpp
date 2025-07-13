// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CEquipment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"


void UCEquipment::BeginPlay(ACharacter* InOwner, const FEquipmentData& InData)
{
	OwnerCharacter = InOwner;
	Data = InData;

	Movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);
	State = CHelpers::GetComponent<UCStateComponent>(InOwner);
}

void UCEquipment::Equip_Implementation()
{
	State->SetEquipMode();

	if (Data.bCanMove == false)
	{
		Movement->Stop();
	}

	if (Data.bUseControlRotation)
	{
		Movement->EnableControlRotation();
	}

	if (!!Data.Montage)
	{
		OwnerCharacter->PlayAnimMontage(Data.Montage,Data.PlatRate);
	}
	else
	{
		Begin_Equip();
		End_Equip();
		
		//로그 띄우기 , 없는 경우 경고 띄우기용
		GLog->Log(ELogVerbosity::Error,"Equip Montage == nullptr");
	}
}

void UCEquipment::UnEquip_Implementation()
{
	bEquipped = false;
	
	if (Data.bUseControlRotation)
	{
		Movement->DisableControlRotation();
	}
	if (OnEquipmentUnEquip.IsBound())
	{
		OnEquipmentUnEquip.Broadcast();
	}
	
}

void UCEquipment::Begin_Equip_Implementation()
{
	bBeginEquip = true;
	
	if (OnEquipmentBeginEquip.IsBound())
	{
		OnEquipmentBeginEquip.Broadcast();
	}
}

void UCEquipment::End_Equip_Implementation()
{
	bBeginEquip = false;
	bEquipped = true;
	
	if (Data.bCanMove == false)
	{
		Movement->Move();
	}
	State->SetIdleMode();
}

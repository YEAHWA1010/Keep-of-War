// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CWeaponAsset.h"
#include "Global.h"
#include "CAttachment.h"
#include "GameFramework/Character.h"
#include "CEquipment.h"
#include "CDoAction.h"
#include "CSubAction.h"
#include "CWeaponData.h"


UCWeaponAsset::UCWeaponAsset()
{
	//데이터 에셋 널값 방지용 기본값 넣어주기
	//여기에 지정한 블루프린트 클래스를 스판시키는 것
	AttachmentClass = ACAttachment::StaticClass();
	EquipmentClass = UCEquipment::StaticClass();
}

void UCWeaponAsset::BeginPlay(ACharacter* InOwner,UCWeaponData** OutWeaponData)
{
	if (!!AttachmentClass)
	{
		FActorSpawnParameters params;
		params.Owner = InOwner;

		//지정한 블루프린트 클래스를 스폰시킬예정
		Attachment = InOwner->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, params);
	}

	if (!!EquipmentClass) //화면에 등장안하고 메모리에만 할당
	{
		//EquipmentClass의 타입으로 생성해서 Equipment에 저장
		//직렬화로 가비지 콜렉터로 관리되는애는 newobject로 동적 할당함 
		Equipment = NewObject<UCEquipment>(this,EquipmentClass);
		Equipment -> BeginPlay(InOwner,EquipmentData);

		if (!!Attachment)
		{
			//Attachment객체의OnBeginEquip연결
			Equipment->OnEquipmentBeginEquip.AddDynamic(Attachment,&ACAttachment::OnBeginEquip);
			Equipment->OnEquipmentUnEquip.AddDynamic(Attachment,&ACAttachment::OnUnEquip);
		}
	}
	
	if (!!DoActionClass)
	{
		DoAction = NewObject<UCDoAction>(this,DoActionClass);
		DoAction->BeginPlay(InOwner,Attachment,Equipment,DoActionDatas, HitDatas);

		if (!!Attachment)
		{
			Attachment->OnFAttachmentBeginCollision.AddDynamic(DoAction,&UCDoAction::UCDoAction::OnAttachmentBeginCollision);
			Attachment->OnFAttachmentEndCollision.AddDynamic(DoAction,&UCDoAction::UCDoAction::OnAttachmentEndCollision);

			Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction,&UCDoAction::UCDoAction::OnAttachmentBeginOverlap);
			Attachment->OnAttachmentEndOverlap.AddDynamic(DoAction,&UCDoAction::UCDoAction::OnAttachmentEndOverlap);
		}

		if (!!Equipment)
		{
			//활
			Equipment->OnEquipmentBeginEquip.AddDynamic(DoAction,&UCDoAction::OnBeginEquip);
			Equipment->OnEquipmentUnEquip.AddDynamic(DoAction,&UCDoAction::OnUnEquip);
		}
	}

	SubAction = nullptr;
	if (!!SubActionClass)
	{
		SubAction = NewObject<UCSubAction>(this,SubActionClass);
		SubAction->BeginPlay(InOwner,Attachment,DoAction);
	}

	//캐릭터 각각 마다 이걸 가지고 있음
	*OutWeaponData = NewObject<UCWeaponData>();
	(*OutWeaponData)->Attachment = Attachment;
	(*OutWeaponData)->Equipment = Equipment;
	(*OutWeaponData)->DoAction = DoAction;
	(*OutWeaponData)->SubAction = SubAction;
}


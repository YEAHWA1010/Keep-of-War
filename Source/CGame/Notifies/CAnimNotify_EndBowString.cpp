// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_EndBowString.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/DoActions/CDoAction_Bow.h"

FString UCAnimNotify_EndBowString::GetNotifyName_Implementation() const
{
	return "EndBowString";
}

void UCAnimNotify_EndBowString::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	//상속받으면 무조건 super 콜 해줘야 함
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetDoAction());

	UCDoAction_Bow* bow = Cast<UCDoAction_Bow>(weapon->GetDoAction());
	CheckNull(bow);

	bow->End_BowString();
}
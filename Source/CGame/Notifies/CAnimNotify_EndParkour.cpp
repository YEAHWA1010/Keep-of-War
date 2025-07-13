// Fill out your copyright notice in the Description page of Project Settings.

#include "Notifies/CAnimNotify_EndParkour.h"
#include "Global.h"
#include "Components/CParkourComponent.h"

FString UCAnimNotify_EndParkour::GetNotifyName_Implementation() const
{
	return "EndParkour";
}

void UCAnimNotify_EndParkour::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	//상속받으면 무조건 super 콜 해줘야 함
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCParkourComponent* Parkour = CHelpers::GetComponent<UCParkourComponent>(MeshComp->GetOwner());
	CheckNull(Parkour);
	Parkour->End_DoParkour();
}
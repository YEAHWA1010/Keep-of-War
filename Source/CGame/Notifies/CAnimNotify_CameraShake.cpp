// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_CameraShake.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Camera/CameraShakeBase.h"

FString UCAnimNotify_CameraShake::GetNotifyName_Implementation() const
{
	return "CameraShake";
}

void UCAnimNotify_CameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	//상속받으면 무조건 super 콜 해줘야 함
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(Character); //캐릭터 없으면 안됨
	
	//템플릿 방식 (UE4.25 이후 권장 방식)
	APlayerController* Controller = Character->GetController<APlayerController>();
	CheckNull(Controller);

	Controller->PlayerCameraManager->StartCameraShake(CameraShakeClass);
}
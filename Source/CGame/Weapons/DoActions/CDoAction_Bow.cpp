// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/DoActions/CDoAction_Bow.h"
#include "Weapons/Attachments/CAttachment_Bow.h"
#include "GameFramework/PlayerController.h"
#include "Global.h"
#include "Weapons/AddOns/CArrow.h"
#include "Weapons/CEquipment.h"
#include "Components/CStateComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"


UCDoAction_Bow::UCDoAction_Bow()
{
	
}

void UCDoAction_Bow::OnBeginEquip()
{
	Super::OnBeginEquip();

	CreateArrow();
}

void UCDoAction_Bow::OnUnEquip()
{
	Super::OnUnEquip();
	
	//수정할때는 월드로 잡아줌
	PoseableMesh->SetBoneLocationByName("bow_String_mid",OriginLocation, EBoneSpaces::ComponentSpace);
	
	for (int i = Arrows.Num() - 1; i >= 0; i--)
	{
		ACArrow* arrow = Arrows[i];

		if (!!arrow)
		{
			if (!!Arrows[i]->GetAttachParentActor())
			{
				Arrows.Remove(arrow); //배열에서 객체를 제거
				arrow->Destroy();
			}
		}
	}
}

void UCDoAction_Bow::End_BowString()
{
	//끝날때 다시 붙여줌
	*Bending = 100.0f;
	bAttachedString = true;
}

void UCDoAction_Bow::BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment,UCEquipment* InEquipment,
                               const TArray<FDoActionData>& InDoActionDatas, const TArray<FHitData>& InHitDatas)
{
	Super::BeginPlay(InOwner, InAttachment,InEquipment,InDoActionDatas, InHitDatas);
	SkeletalMesh = CHelpers::GetComponent<USkeletalMeshComponent>(InAttachment);
	PoseableMesh = CHelpers::GetComponent<UPoseableMeshComponent>(InAttachment);

	//원래 상대 위치가 나옴
	OriginLocation = PoseableMesh->GetBoneLocation("bow_String_mid",EBoneSpaces::ComponentSpace);

	bEquiped = InEquipment->GetEquipped();

	ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);
	Bending = bow->GetAnimInstance_Bending();
}

void UCDoAction_Bow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//캡쳐한 다음에 수정
	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);
	
	//장착이 완료되어 true 일 경우에만 붙이기
	CheckFalse(*bEquiped);
	CheckFalse(bAttachedString);
	
	FVector HandLocation = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Right");
	//SetBoneLocationByName는 어떤 본을 수정할거냐는 것
	//들어가는애가 월드 공간에서 구해진 애냐 컴포넌트 공간에서 구해진 애냐의 차이
	PoseableMesh->SetBoneLocationByName("bow_string_mid",HandLocation,EBoneSpaces::WorldSpace);
}

void UCDoAction_Bow::DoAction()
{
	CheckFalse(State->IsIdleMode());
	//CheckFalse(State->IsSubActionMode());
	
	Super::DoAction();

	DoActionDatas[0].DoAction(OwnerCharacter);
}

void UCDoAction_Bow::Begin_DoAction()
{
	Super::Begin_DoAction();

	*Bending = 0.0f;
	bAttachedString = false;
	
	ACArrow* arrow = GetAttachedArrow();
	CheckNull(arrow);
	arrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld,true));

	arrow->OnHit.AddDynamic(this,&UCDoAction_Bow::OnArrowHit);
	arrow->OnEndPlay.AddDynamic(this,&UCDoAction_Bow::OnArrowEndPlay);
	
	FVector Forward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();
	arrow->Shoot(Forward);

}

void UCDoAction_Bow::End_DoAction()
{
	Super::End_DoAction();

	CreateArrow();
}

void UCDoAction_Bow::OnArrowHit(class AActor* InCauser, class ACharacter* InOtherCharacter)
{
	CheckFalse(HitDatas.Num() > 0);

	HitDatas[0].SendDamage(OwnerCharacter,InCauser,InOtherCharacter);
}

void UCDoAction_Bow::OnArrowEndPlay(class ACArrow* InDestroyer)
{
	Arrows.Remove(InDestroyer); //해당 객체 찾아서 삭제해줌
}

void UCDoAction_Bow::CreateArrow()
{
	//게임 종료 중 , 생성할때 종료이면 하지 않도록
	CheckTrue(World->bIsTearingDown);
	CheckNull(ArrowClass);
	
	//생성만 된거지 배치 아님
	FTransform Transform;
	ACArrow* Arrow = World->SpawnActorDeferred<ACArrow>(ArrowClass,Transform,nullptr,nullptr,ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	CheckNull(Arrow);

	Arrow->AddIgonreActor(OwnerCharacter);

	FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	Arrow->AttachToComponent(OwnerCharacter->GetMesh(),rule,"Hand_Bow_Right_Arrow");

	Arrows.Add(Arrow);
	//확정된 배치 위치
	UGameplayStatics::FinishSpawningActor(Arrow,Transform);
}
ACArrow* UCDoAction_Bow::GetAttachedArrow()
{
	for (ACArrow* arrow : Arrows)
	{
		if (!!arrow->GetAttachParentActor())
		{
			return arrow; //붙어있는화살
		}
	}

	return nullptr;
}
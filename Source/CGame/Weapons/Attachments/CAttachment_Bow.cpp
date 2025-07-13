// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Attachments/CAttachment_Bow.h"
#include "Global.h"
#include "Characters/CAnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "UniversalObjectLocators/AnimInstanceLocatorFragment.h"
#include "Weapons/AnimInstances/CAnimInstance_Bow.h"

float* ACAttachment_Bow::GetAnimInstance_Bending()
{
	//실제 애니메이션 객체가 리턴됨
	UCAnimInstance_Bow* bow = Cast<UCAnimInstance_Bow>(SkeletalMesh->GetAnimInstance()); //넣은 애니메이션 객체가 리턴

	if (!!bow)
	{
		return bow->GetBending(); 
	}

	return nullptr;
}

ACAttachment_Bow::ACAttachment_Bow()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USkeletalMeshComponent>(this,&SkeletalMesh,"SkeletalMesh",Root);
	CHelpers::CreateComponent<UPoseableMeshComponent>(this,&PoseableMesh,"PoseableMesh",Root);

	USkeletalMesh* Mesh;
	CHelpers::GetAsset<USkeletalMesh>(&Mesh,"SkeletalMesh'/Game/Characters/Weapons/ElvenBow/SK_ElvenBow.SK_ElvenBow'");
	SkeletalMesh->SetSkeletalMesh(Mesh);
	SkeletalMesh->SetCollisionProfileName("NoCollision");

	TSubclassOf<UCAnimInstance_Bow> animInstance;
	CHelpers::GetClass<UCAnimInstance_Bow>(&animInstance,"AnimBlueprint'/Game/Characters/Weapons/ElvenBow/ABP_Bow.ABP_Bow_C'");
	SkeletalMesh->SetAnimInstanceClass(animInstance);
}

void ACAttachment_Bow::OnBeginEquip_Implementation()
{
	Super::OnBeginEquip_Implementation();

	AttachTo("Hand_Bow_Left");

	APlayerController* Controller = OwnerCharacter->GetController<APlayerController>();
	CheckNull(Controller);

	//현재 pitch의 앵글(view ,min, max) 을 저장 => 복구하기 위해
	OriginViewPitchRange.X = Controller->PlayerCameraManager->ViewPitchMin;
	OriginViewPitchRange.Y = Controller->PlayerCameraManager->ViewPitchMax;

	Controller->PlayerCameraManager->ViewPitchMin = ViewPitchRange.X;
	Controller->PlayerCameraManager->ViewPitchMax = ViewPitchRange.Y;
}

void ACAttachment_Bow::OnUnEquip_Implementation()
{
	Super::OnUnEquip_Implementation();

	AttachTo("Holster_Bow");

	APlayerController* Controller = OwnerCharacter->GetController<APlayerController>();
	CheckNull(Controller);

	Controller->PlayerCameraManager->ViewPitchMin = OriginViewPitchRange.X;
	Controller->PlayerCameraManager->ViewPitchMax = OriginViewPitchRange.Y;
}

void ACAttachment_Bow::BeginPlay()
{
	Super::BeginPlay();

	AttachTo("Holster_Bow");
	SkeletalMesh->SetVisibility(false);

	//Mesh를 기준으로 애니메이션을 캡쳐해주면서 넣어줌
	PoseableMesh->SetSkeletalMesh(SkeletalMesh->SkeletalMesh); //이 Mesh임 , 애니메이션을 캡쳐하면서(정확히는 본들을 캡쳐) 움직일 것
	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh); //Copy까지 해야 캡쳐 준비가 됨
}

void ACAttachment_Bow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CAnimInstance.h"
#include "Global.h"
#include "KismetAnimationLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/SubActions/CSubAction_Bow.h"
#include "Characters/CPlayer.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner()); //Owner 캐릭터 받아오기
	CheckNull(OwnerCharacter);

	Weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);

	if (!!Weapon)
	{
		Weapon->OnWeaponTypeChanged.AddDynamic(this,&UCAnimInstance::OnWeaponTypeChanged);
	}
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CheckNull(OwnerCharacter); //if(OwnerCharacter == nullptr) return;
	
	Speed = OwnerCharacter->GetVelocity().Size2D();
	//Direction = UKismetAnimationLibrary::CalculateDirection(OwnerCharacter->GetVelocity(),OwnerCharacter->GetControlRotation());

	//이 방향 벡터가 향하는 회전값이 나옴
	FRotator rotator = OwnerCharacter->GetVelocity().ToOrientationRotator();
	FRotator rotator2 = OwnerCharacter->GetControlRotation();
	//두개의 각도차 , 수직 방어 완료
	FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(rotator, rotator2);
	//보간이 들어가서 끊어지는 듯한 느낌이 아예 없음
	PreRotation = UKismetMathLibrary::RInterpTo(PreRotation,delta,DeltaSeconds,25);
	Direction = PreRotation.Yaw;
	//GetBaseRotationOffset() 조준 회전값 => 카메라의 시야와 플레이어의 정면의 중간값(조준할 값) , 25 = 시야 25
	Pitch = UKismetMathLibrary::FInterpTo(Pitch,OwnerCharacter->GetBaseAimRotation().Pitch,DeltaSeconds,25);

	//이전에 저장해둔 pitch랑 현재 pitch값을 구함
	bFalling = OwnerCharacter->GetCharacterMovement()->IsFalling();
	
	CheckNull(Weapon);
	
	if (!!Weapon ->GetSubAction())
	{
		bBow_Aiming = true;
		//둘 다 true여야함
		bBow_Aiming &= WeaponType == EWeaponType::Bow;
		bBow_Aiming &= Weapon->GetSubAction()->GetInAction();
	}
}

void UCAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}

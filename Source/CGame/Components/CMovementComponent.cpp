// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/CMovementComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"


#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UCMovementComponent::UCMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwnerCharacter = Cast<ACharacter>(GetOwner()); //얘를 생성시키는애를 GetOwner
}

void UCMovementComponent::SetSpeed(ESpeedType InType)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed[(int32)InType];
}

void UCMovementComponent::OnSprint()
{
	SetSpeed(ESpeedType::Sprint);
}

void UCMovementComponent::OnRun()
{
	SetSpeed(ESpeedType::Run);
}

void UCMovementComponent::OnWalk()
{
	SetSpeed(ESpeedType::Walk);
}

void UCMovementComponent::Move()
{
	bCanMove = true;
}

void UCMovementComponent::Stop()
{
	bCanMove = false;
}

void UCMovementComponent::OnMoveForward(float InAxis)
{
	CheckFalse(bCanMove);

	//카메라의 방향
	FRotator rotator = FRotator(0,OwnerCharacter->GetControlRotation().Yaw,  0);
	//쿼터니온으로 변환
	FVector direction = FQuat(rotator).GetForwardVector();

	OwnerCharacter->AddMovementInput(direction,InAxis);
}

void UCMovementComponent::OnMoveRight(float InAxis)
{
	CheckFalse(bCanMove);

	//카메라의 방향
	FRotator rotator = FRotator(0,OwnerCharacter->GetControlRotation().Yaw,  0);
	//쿼터니온으로 변환
	FVector direction = FQuat(rotator).GetRightVector();

	OwnerCharacter->AddMovementInput(direction,InAxis);
}

void UCMovementComponent::OnHorizontalLook(float InAxis)
{
	//true면 밑에 안하고 카메라 고정
	CheckTrue(bFixedCamera);
	
	//마우스 속도 조정
	OwnerCharacter->AddControllerYawInput(InAxis * HorizontalLook * GetWorld()->GetDeltaSeconds());
}

void UCMovementComponent::OnVerticalLook(float InAxis)
{
	//true면 밑에 안하고 카메라 고정
	CheckTrue(bFixedCamera);
	
	OwnerCharacter->AddControllerPitchInput(InAxis * VerticalLook * GetWorld()->GetDeltaSeconds());
}

void UCMovementComponent::EnableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
}

void UCMovementComponent::DisableControlRotation()
{
	//컨트롤러의 회전값을 따름
	OwnerCharacter->bUseControllerRotationYaw = false;
	//이동하는 방향으로 회전
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}


// Called every frame
void UCMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


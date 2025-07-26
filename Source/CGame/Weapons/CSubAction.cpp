// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CSubAction.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CapsuleComponent.h"

UCSubAction::UCSubAction()
{
	
}

void UCSubAction::BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction)
{
	Owner = InOwner;
	Attachment = InAttachment;
	DoAction = InDoAction;

	State = CHelpers::GetComponent<UCStateComponent>(Owner);
	Movement = CHelpers::GetComponent<UCMovementComponent>(Owner);
}

void UCSubAction::Tick(float DeltaTime)
{
	
}

void UCSubAction::Pressed()
{
	bInAction = true;
}

void UCSubAction::Released()
{
	bInAction = false;
}


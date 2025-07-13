// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CStateComponent.h"

// Sets default values for this component's properties
UCStateComponent::UCStateComponent()
{

}


void UCStateComponent::ChangeType(EStateType InType)
{
	EStateType prevType = Type;
	//현재타입
	Type = InType;

	//IsBound() = 연결된게 있다면
	if (OnStateTypeChanged.IsBound())
	{
		if (OnStateTypeChanged.IsBound())
		{
			OnStateTypeChanged.Broadcast(prevType,InType);
		}
	}
}

// Called when the game starts
void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}

void UCStateComponent::SetEvadeMode()
{
	ChangeType(EStateType::Evade);
}

void UCStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);
}

void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UCStateComponent::SetDamagedMode()
{
	ChangeType(EStateType::Damaged);
}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UCStateComponent::OnSubActionMode()
{
	bInSubActionMode = true;
}

void UCStateComponent::OffSubActionMode()
{
	bInSubActionMode = false;
}



// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

//블프에 붙이는 애들은 UEnum으로 클래스 위에 있어야 한다
UENUM()
enum class EStateType : uint8
{
	Idle = 0,
	Evade,Equip,Damaged,Action,Dead,Max,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged,EStateType,InPrevType,EStateType,InNewType);

//델리게이트 정의 , Multicast => 여러개 정의 가능
//Dynamic은 블프에서도 사용 가능
UCLASS()
class CGAME_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCStateComponent();
	FStateTypeChanged OnStateTypeChanged;

	//Idle모드인지 체크
	FORCEINLINE bool IsIdleMode() {return Type == EStateType::Idle;}
	FORCEINLINE bool IsDeadMode() {return Type == EStateType::Dead;}
	FORCEINLINE bool IsDamagedMode() {return Type == EStateType::Damaged;}
	FORCEINLINE bool IsSubActionMode() {return bInSubActionMode; }

private:
	void ChangeType(EStateType InType);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void SetIdleMode();
	void SetEvadeMode();
	void SetEquipMode();
	void SetActionMode();
	void SetDamagedMode();
	void SetDeadMode();

	void OnSubActionMode();
	void OffSubActionMode();

private:
	EStateType Type;

private:
	bool bInSubActionMode;
};

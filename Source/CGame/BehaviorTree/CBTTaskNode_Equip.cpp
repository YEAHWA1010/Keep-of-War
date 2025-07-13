// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CBTTaskNode_Equip.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Components/CStateComponent.h"
#include "Weapons/CEquipment.h"

UCBTTaskNode_Equip::UCBTTaskNode_Equip()
{
	NodeName = "Equip";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Equip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* AI = Cast<ACEnemy_AI>(Controller->GetPawn());

	UCWeaponComponent* Weapon = CHelpers::GetComponent<UCWeaponComponent>(AI);
	CheckNullResult(Weapon, EBTNodeResult::Failed);
	CheckTrueResult(Type == EWeaponType::Max, EBTNodeResult::Failed);

	if (Type == Weapon->GetType())
		return EBTNodeResult::Succeeded;

	switch (Type)
	{
	case EWeaponType::Sword: Weapon->SetSwordMode(); break;
	case EWeaponType::Warp: Weapon->SetWarpMode(); break;
	case EWeaponType::Bow: Weapon->SetBowMode(); break;
	}

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Equip::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* AI = Cast<ACEnemy_AI>(Controller->GetPawn());

	UCWeaponComponent* Weapon = CHelpers::GetComponent<UCWeaponComponent>(AI);

	const bool* bEquipped = Weapon->GetEquipment()->GetEquipped();

	UCStateComponent* State = CHelpers::GetComponent<UCStateComponent>(AI);
	
	if (*bEquipped && State->IsIdleMode())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	
}

EBTNodeResult::Type UCBTTaskNode_Equip::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* AI = Cast<ACEnemy_AI>(Controller->GetPawn());

	UCWeaponComponent* Weapon = CHelpers::GetComponent<UCWeaponComponent>(AI);

	//equip하다가 맞을 경우
	bool bBeginEquip = Weapon->GetEquipment()->GetBeginEquip();
	
	if (bBeginEquip == false) //장착이 호출되지 않은 경우
		Weapon->GetEquipment()->Begin_Equip();

	Weapon->GetEquipment()->End_Equip(); //중단된경우

	return EBTNodeResult::Aborted;
}

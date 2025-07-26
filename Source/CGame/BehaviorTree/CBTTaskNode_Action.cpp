// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CBTTaskNode_Action.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CDoAction.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	NodeName = "Action";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* AI = Cast<ACEnemy_AI>(Controller->GetPawn());
	//CheckNull(AI);
	UCWeaponComponent* Weapon = CHelpers::GetComponent<UCWeaponComponent>(AI);
	//CheckNull(Weapon);
	CheckNullResult(Weapon, EBTNodeResult::Failed);
	CheckTrueResult(Weapon->IsUnarmedMode(), EBTNodeResult::Failed);

	Controller->StopMovement();
	Weapon->DoAction();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* AI = Cast<ACEnemy_AI>(Controller->GetPawn());

	UCWeaponComponent* Weapon = CHelpers::GetComponent<UCWeaponComponent>(AI);
	UCStateComponent* State = CHelpers::GetComponent<UCStateComponent>(AI);
	
	bool bCheck = true;
	bCheck &= (State->IsIdleMode());
	bCheck &= (Weapon->GetDoAction()->GetInAction() == false);

	if (bCheck)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

EBTNodeResult::Type UCBTTaskNode_Action::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);
	
	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* AI = Cast<ACEnemy_AI>(Controller->GetPawn());

	UCWeaponComponent* Weapon = CHelpers::GetComponent<UCWeaponComponent>(AI);

	bool bBeginAction = Weapon->GetDoAction()->GetBeginAction();
	
	if (bBeginAction == false)
		Weapon->GetDoAction()->Begin_DoAction();

	Weapon->GetDoAction()->End_DoAction();

	return EBTNodeResult::Succeeded;
}

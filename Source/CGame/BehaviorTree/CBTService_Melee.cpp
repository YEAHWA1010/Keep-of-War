// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CBTService_Melee.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CStateComponent.h"

UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Melee";

	Interval = 0.1f;
	RandomDeviation = 0.0f;
}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* AI = Cast<ACEnemy_AI>(Controller->GetPawn());

	UCAIBehaviorComponent* BehaviorComponent = CHelpers::GetComponent<UCAIBehaviorComponent>(AI);
	UCStateComponent* State = CHelpers::GetComponent<UCStateComponent>(AI);

	if (State->IsDamagedMode())
	{
		BehaviorComponent->SetHittedMode();

		return;
	}
	//CLog::Print(AI->GetName());

	//AI상태를 설정
	ACharacter* Target = BehaviorComponent->GetTarget();
	if (Target == nullptr)
	{
		//대기
		//BehaviorComponent->SetWaitMode();
		BehaviorComponent->SetPatrolMode();
		return;
	}

	//ai가 비헤이비어를 운영 중이고, 타겟하고의 거리를 구한다
	float Distance = AI->GetDistanceTo(Target);
	if (Distance < ActionRange)
	{
		//액션
		BehaviorComponent->SetActionMode();
		return;
	}

	BehaviorComponent->SetApproachMode();
}

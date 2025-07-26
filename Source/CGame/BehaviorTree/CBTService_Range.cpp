// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CBTService_Range.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CStateComponent.h"

UCBTService_Range::UCBTService_Range()
{
	NodeName = "Range";

	Interval = 0.1f;
	RandomDeviation = 0.0f;
}

void UCBTService_Range::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* AI = Cast<ACEnemy_AI>(Controller->GetPawn());

	UCAIBehaviorComponent* BehaviorComponent = CHelpers::GetComponent<UCAIBehaviorComponent>(AI);
	UCStateComponent* State = CHelpers::GetComponent<UCStateComponent>(AI);
	if (bDrawDebug)
	{
		FVector Start = AI->GetActorLocation();
		Start.Z -= 25.0f;

		FVector End = Start;
		End.Z += 50.0f;

		DrawDebugCylinder(AI->GetWorld(),Start,End,AvoidRange,10,FColor::Red,false,Interval);
	}

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
		Controller->ClearFocus(EAIFocusPriority::Gameplay);
		BehaviorComponent->SetWaitMode();
		
		return;
	}

	//타깃 바라보기
	Controller->SetFocus(Target);

	//ai가 비헤이비어를 운영 중이고, 타겟하고의 거리를 구한다
	float Distance = AI->GetDistanceTo(Target);
	if (Distance < AvoidRange)
	{
		//액션
		BehaviorComponent->SetAvoidMode();
		return;
	}

	BehaviorComponent->SetActionMode();
}

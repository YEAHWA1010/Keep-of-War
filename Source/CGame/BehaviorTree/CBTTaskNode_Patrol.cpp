// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CBTTaskNode_Patrol.h"

#include "AsyncTreeDifferences.h"
#include "CPatrolPath.h"
#include "Global.h"
#include "HeadMountedDisplayTypes.h"
#include "Components/CAIBehaviorComponent.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"


UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
	NodeName = "Patrol";

	bNotifyTick = true; //이걸 켜야 틱을 실행함
}

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* AI = Cast<ACEnemy_AI>(Controller->GetPawn());
	UCAIBehaviorComponent* Behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(AI);

	if (!!AI->GetPatrolPath())
	{
		FVector moveToPoint = AI->GetPatrolPath()->GetMoveTo();
		Behavior->SetPatrolLocation(moveToPoint);

		if (bDebugMode)
			DrawDebugSphere(AI->GetWorld(), moveToPoint, 10, 10, FColor::Green, false, 5);

		return EBTNodeResult::InProgress;
	}

	
	FVector Location = AI->GetActorLocation();
	//UNavigationSystemV1 : 네비메쉬 바운딩이 리턴된다.
	UNavigationSystemV1* Navigation = FNavigationSystem::GetCurrent<UNavigationSystemV1>(AI->GetWorld());

	CheckNullResult(Navigation,EBTNodeResult::Failed);

	FNavLocation Point(Location); //구조체 자료형
	
	//GetRandomPoint 네비게이션과 상관없이 갈 수 있는 곳을 리턴
	//GetRandomPointInNavigableRadius 지정한 곳을 갈 수 없으면 false
	
	while (true)
	{
		if (Navigation->GetRandomPointInNavigableRadius(Location,RandomRadius,Point))
		{
			break;
		}
	}

	Behavior->SetPatrolLocation(Point.Location); //랜덤으로 가려는 위치를 구한게 point.Location임

	if (bDebugMode)
	{
		//갈려는 곳에 디버깅
		DrawDebugSphere(Controller->GetWorld(), Point.Location, 10, 10, FColor::Green, true,5);
	}

	return EBTNodeResult::InProgress;
	
}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* AI = Cast<ACEnemy_AI>(Controller->GetPawn());
	UCAIBehaviorComponent* Behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(AI);

	//가려는 위치를 가져옴
	FVector Location = Behavior->GetPatrolLocation();
	//MoveToLocation : 위치를 주는 경우
	//MoveToActor : 액터를 줌

	//MoveToLocation -> 비헤이비어에 있는 moveto가 이거 기반
	EPathFollowingRequestResult::Type Result = Controller->MoveToLocation(Location,GoalDistance,false); //얼마 근처에 가면 도달했는지 , bStopOnOverlap : 위치에 겹치는 액터가 있었으면 어떻게 할거냐

	switch (Result)
	{
		case EPathFollowingRequestResult::Type::Failed: //종료 시
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed); //갈 수 없는곳은 failed
				break;
			}
		case EPathFollowingRequestResult::Type::AlreadyAtGoal: //이미 도착 시
			{
				if (!!AI->GetPatrolPath())
					AI->GetPatrolPath()->UpdateIndex();
				
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				break;
			}
	}
}

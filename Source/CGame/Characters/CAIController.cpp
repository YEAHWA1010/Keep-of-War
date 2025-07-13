// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/CAIController.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CEnemy_AI.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CAIBehaviorComponent.h"

ACAIController::ACAIController()
{
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this,&Perception,"Perception");
	CHelpers::CreateActorComponent<UBlackboardComponent>(this,&Blackboard,"Blackboard");
	
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 600;
	Sight->LoseSightRadius = 800; //시야 감지를 벗어나면 감지를 잃는다
	Sight->PeripheralVisionAngleDegrees = 45;//시야 좌우 각
	Sight->SetMaxAge(2); //감지를 벗어나면 잃기 시작하는 시간 , 감지 벗어나면 2초뒤 감지 잃음

	Sight->DetectionByAffiliation.bDetectEnemies = true; //적군인지 아군인지 , 적은 감지
	Sight->DetectionByAffiliation.bDetectNeutrals = false; //중립
	Sight->DetectionByAffiliation.bDetectFriendlies = false;

	Perception->ConfigureSense(*Sight); //배열이어서 *붙여서 넣어줌 , 배열이 아니라 객체로 들어감
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
}

void ACAIController::OnPossess(APawn* InPawn) //pawn에 빙의된 캐릭터 들어옴
{
	Super::OnPossess(InPawn);
	//CLog::Log(InPawn->GetName());

	Enemy = Cast<ACEnemy_AI>(InPawn);
	SetGenericTeamId(Enemy->GetTeamID());

	CheckNull(Enemy->GetBehaviorTree());
	
	//비헤이비어에 있는 블랙보드를 여기 만든 블랙보드컴포넌트에 사용할거라고 명시
	//Get을 이용해서 스마트포인터에 있던 포인터를 리턴해줘서 주소를 가져와 그 주소를 넣어줌
	//주소에 대한 레퍼런스를 인자로 받음
	UBlackboardComponent* blackboard = Blackboard.Get();
	UseBlackboard(Enemy->GetBehaviorTree()->BlackboardAsset,blackboard);

	BehaviorComponent = CHelpers::GetComponent<UCAIBehaviorComponent>(Enemy);
	BehaviorComponent->SetBlackboard(Blackboard);
	
	//비헤이비어트리 실행
	RunBehaviorTree(Enemy->GetBehaviorTree());
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	//매프레임 강제 업데이트
	Perception->OnPerceptionUpdated.AddDynamic(this,&ACAIController::OnPerceptionUpdated);
}

void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> actors;
	//UpdatedActors얘는 감지된 액터가 누적으로 들어와서 이걸 쓰지 않고
	//tsubofclass인 경우 static으로 받음
	Perception->GetCurrentlyPerceivedActors(nullptr,actors);//nullptr를 쓰면 어떤거든 받아옴 , 어떤 방식으로 감지하는 애를 넣을지를 인자에 넣음

	// for (AActor* actor : actors)
	// {
	// 	CLog::Print(actor->GetName());
	// }

	if (actors.Num() > 0)
	{
		Blackboard->SetValueAsObject("Target",actors[0]); //블랙보드 값을 세팅
		return;
	}

	//감지가 안된 경우 타깃 비우기
	Blackboard->SetValueAsObject("Target",nullptr);
}

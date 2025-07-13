// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CEnvQueryContext_Target.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CAIBehaviorComponent.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

void UCEnvQueryContext_Target::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	//타깃이라는 이름으로 쿼리어에 나타남
	
	//QueryInstance.Owner => Owner는 QueryContext를 실행시키는 애 => 적
	//QueryContext는 쿼리 그래프의 제너레이트에서 선택된 애들
	ACEnemy_AI* AI = Cast<ACEnemy_AI>(QueryInstance.Owner.Get()); //이렇게 하면 일반 포인터로 캐스팅 GET을 사용하면 일반 포인터리턴
	UCAIBehaviorComponent* BehaviorComponent = CHelpers::GetComponent<UCAIBehaviorComponent>(AI);

	//설정할 타입
	UEnvQueryItemType_Actor::SetContextHelper(ContextData,BehaviorComponent->GetTarget());
	
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_EndState.h"
#include "Global.h"
#include "Characters/IStateable.h"

FString UCAnimNotify_EndState::GetNotifyName_Implementation() const
{
	return "EndState";
}

void UCAnimNotify_EndState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	//상속받으면 무조건 super 콜 해줘야 함
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IIStateable* state = Cast<IIStateable>(MeshComp->GetOwner());
	CheckNull(state);

	switch (StateType)
	{
		case EStateType::Evade: state->End_BackStep(); break;
		case EStateType::Dead: state->End_Dead(); break;
		case EStateType::Damaged: state->End_Damaged(); break;
	}
	//Player->End_BackStep();
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState_Combo.generated.h"

/**
 * 
 */
UCLASS()
class CGAME_API UCAnimNotifyState_Combo : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	FString GetNotifyName_Implementation() const override;

	//virtual 안쓰면 자식에서 재정의 불가능
	void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;
};

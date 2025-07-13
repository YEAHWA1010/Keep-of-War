// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_CameraShake.generated.h"

/**
 * 
 */
UCLASS()
class CGAME_API UCAnimNotify_CameraShake : public UAnimNotify
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "CameraClass")
	TSubclassOf<class UCameraShakeBase> CameraShakeClass;

	//변수 아니고 타입은 TSubclassof로 넣어줌
	//TSubclassOf<class UMatineeCameraShake> CameraShakeClass;
private:
	FString GetNotifyName_Implementation() const override;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};

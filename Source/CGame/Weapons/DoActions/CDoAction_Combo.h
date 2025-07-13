// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Combo.generated.h"

/**
 * 
 */
UCLASS()
class CGAME_API UCDoAction_Combo : public UCDoAction
{
	GENERATED_BODY()
public:
	//외부 노티파이를 통해 들어올 애들
	FORCEINLINE void EnableCombo() {bEnable = true;}
	FORCEINLINE void DisableCombo() {bEnable = false;}

public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

	//재정의는 UFUNCTION 필요 X virtual도 필요 X
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther) override; 
	void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) override; 
	
	void OnAttachmentEndCollision() override;
	
public:
	int32 Index;
	
	bool bEnable;
	bool bExist;

private:
	TArray<class ACharacter*> Hitted;
};

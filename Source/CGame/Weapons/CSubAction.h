// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSubAction.generated.h"

UCLASS()
class CGAME_API UCSubAction : public UObject
{
	GENERATED_BODY()

public:
	UCSubAction();
	
public:
	//활성화 시키고 끄고 키는 용도
	FORCEINLINE bool GetInAction() { return bInAction; }
public:
	virtual void BeginPlay(class ACharacter* InOwner,class ACAttachment* InAttachment, class UCDoAction* InDoAction);
	virtual void Tick(float DeltaTime);

public:
	virtual void Pressed();
	virtual void Released();

	bool bInAction;

	class ACharacter* Owner;
	class ACAttachment* Attachment;
	class UCDoAction* DoAction;

	class UCStateComponent* State;
	class UCMovementComponent* Movement;
	
};

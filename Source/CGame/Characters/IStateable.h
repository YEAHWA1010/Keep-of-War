// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IStateable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIStateable : public UInterface
{
	GENERATED_BODY()
};
//위에는 IIStateable를 직렬화 하기 위해 존재 , 건드릴 필요 x

class CGAME_API IIStateable
{
	GENERATED_BODY()
public:
	virtual void End_BackStep() {}
	virtual void End_Dead() {}
	virtual void End_Damaged() {}
};

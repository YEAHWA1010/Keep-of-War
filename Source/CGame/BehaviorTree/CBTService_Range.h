// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Range.generated.h"

/**
 * 
 */
UCLASS()
class CGAME_API UCBTService_Range : public UBTService
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere,Category="Action")
	float AvoidRange = 500;

	UPROPERTY(EditAnywhere,Category="Action")
	bool bDrawDebug;
	
public:
	UCBTService_Range();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Patrol.generated.h"

UCLASS()
class CGAME_API UCBTTaskNode_Patrol : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Patrol")
	bool bDebugMode;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	float GoalDistance = 20;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	float RandomRadius = 1500;

public:
	UCBTTaskNode_Patrol();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

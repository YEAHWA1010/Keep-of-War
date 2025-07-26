// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAIBehaviorComponent.generated.h"

UENUM(BlueprintType)
enum class EAIStateType : uint8
{
	Wait = 0, Approach, Action, Patrol, Hitted, Avoid, Dead, Max,
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIStateTypeChanged, EAIStateType, InPrevType, EAIStateType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CGAME_API UCAIBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere,Category = "Key")
	FName AIStateTypeKey = "AIState";
	
	UPROPERTY(EditAnywhere,Category = "Key")
	FName TargetKey = "Target";

	UPROPERTY(EditAnywhere,Category = "Key")
	FName EqsLocationKey = "EqsLocation";

	UPROPERTY(EditAnywhere,Category = "Key")
	FName PatrolLocationKey = "PatrolLocation";
	
	class UBlackboardComponent* Blackboard;

public:	
	// Sets default values for this component's properties
	UCAIBehaviorComponent();

	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }

	class ACharacter* GetTarget();

	FVector GetPatrolLocation();
	void SetPatrolLocation(const FVector& InLocation);

	FVector GetEqsLocation();
	void SetEqsLocation(const FVector& InLocation);

public:
	bool IsWaitMode();
	bool IsApproachMode();
	bool IsActionMode();
	bool IsPatrolMode();
	bool IsHittedMode();
	bool IsAvoidMode();
	bool IsDeadMode();

public:
	void SetWaitMode();
	void SetApproachMode();
	void SetActionMode();
	void SetPatrolMode();
	void SetHittedMode();
	void SetAvoidMode();
	void SetDeadMode();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void ChangeType(EAIStateType InType);
	EAIStateType GetType();

public:
	FAIStateTypeChanged OnAIStateTypeChanged;

};

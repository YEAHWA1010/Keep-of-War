// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

/**
 * 
 */
UCLASS()
class CGAME_API ACAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACAIController();

private:
	UPROPERTY(VisibleAnywhere)
		class UAIPerceptionComponent* Perception;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	
private:
	class ACEnemy_AI* Enemy; //빙의 받은 애 저장
	class UCAIBehaviorComponent* BehaviorComponent;
	class UAISenseConfig_Sight* Sight;
};

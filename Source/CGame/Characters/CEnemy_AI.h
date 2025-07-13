// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "CEnemy_AI.generated.h"

/**
 * 
 */
UCLASS()
class CGAME_API ACEnemy_AI : public ACEnemy
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	class UCWeaponComponent* Weapon;

	UPROPERTY(VisibleAnywhere)
	class UCAIBehaviorComponent* Behavior;
	
	UPROPERTY(EditDefaultsOnly,Category="Team")
	uint8 TeamID = 2; //몽타주 자료형
	
	UPROPERTY(EditDefaultsOnly,Category="Team")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	class ACPatrolPath* PatrolPath;

private:
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* LabelWidget;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* CursorWidget;

// #if WITH_EDITOR
// private:	
// 	UPROPERTY(VisibleAnywhere)
// 		class UWidgetComponent* LabelWidget;
// #endif
	

protected:
	virtual void BeginPlay() override;
	void Damaged() override;
public:
	virtual void Tick(float DeltaTime) override;
	

public:
	void End_Damaged() override;

public:
	ACEnemy_AI();
	FORCEINLINE uint8 GetTeamID() { return TeamID; }
	
	//컨트롤러에서 받아옴 , 가지고 있는건 AI가 있고 동작은 컨트롤러가 함
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE class ACPatrolPath* GetPatrolPath() { return PatrolPath; }
	
private:
	void Tick_LabelRenderScale();
	
private:
	UFUNCTION()
	void OnHealthPointChanged(float InHealth, float InMaxHealth);
};

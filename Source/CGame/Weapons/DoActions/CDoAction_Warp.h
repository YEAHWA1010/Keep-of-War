// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Warp.generated.h"

/**
 * 
 */
UCLASS()
class CGAME_API UCDoAction_Warp : public UCDoAction
{
	GENERATED_BODY()

public:
	UCDoAction_Warp();

private:
	class UDecalComponent* Decal;
	class APlayerController* PlayerController;
	class UCAIBehaviorComponent* Behavior;

private:
	FVector MoveToLocation; //이동할 위치

public:
	void BeginPlay
	(
		class ACharacter* InOwner,
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		const TArray<FDoActionData>& InDoActionDatas,
		const TArray<FHitData>& InHitDatas
	)override;

	void Tick(float DeltaTime) override;
	void DoAction() override;
	void Begin_DoAction() override;

private:
	//hit된게 있으면 true
	bool GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation);
};

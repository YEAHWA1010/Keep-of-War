// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponStructures.h"
#include "CDoAction.generated.h"

/**
 * 
 */
UCLASS()
class CGAME_API UCDoAction : public UObject
{
	GENERATED_BODY()

public:
	UCDoAction();

	virtual void BeginPlay
	(
		class ACharacter* InOwner,
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		const TArray<FDoActionData>& InDoActionDatas,
		const TArray<FHitData>& InHitDatas
	);

	virtual void Tick(float DeltaTime){}

public:
	virtual void DoAction();
	virtual void Begin_DoAction(); //공격 시작 구간
	virtual void End_DoAction(); //공격 완료 구간
public:
	FORCEINLINE bool GetBeginAction() { return bBeginAction; }
	FORCEINLINE bool GetInAction() { return bInAction; }
	
public:
	UFUNCTION()
		virtual void OnBeginEquip(){};

	UFUNCTION()
		virtual void OnUnEquip(){}; 

	//데미지
	UFUNCTION()
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttachCauser, class ACharacter* InOther) {} //DYNAMIC 델리게이트는 인자까지 똑같아야함
	UFUNCTION()
	virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) {}//DYNAMIC 델리게이트는 인자까지 똑같아야함

	UFUNCTION()
	virtual void OnAttachmentBeginCollision() {}
	UFUNCTION()
	virtual void OnAttachmentEndCollision() {}

protected:
	class ACharacter* OwnerCharacter;
	class UWorld* World;
	
	class UCMovementComponent* Movement;
	class UCStateComponent* State;
	
	TArray<FDoActionData> DoActionDatas;
	TArray<FHitData> HitDatas;

	//공격하다 맞은 경우를 위함
	bool bBeginAction;
	bool bInAction;
};

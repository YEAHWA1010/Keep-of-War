// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Bow.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class CGAME_API UCDoAction_Bow : public UCDoAction
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
		TSubclassOf<class ACArrow> ArrowClass;

private:
	void CreateArrow();
	class ACArrow* GetAttachedArrow(); //붙어있는 화살 찾기
public:
	UCDoAction_Bow();
	void OnBeginEquip() override;
	void OnUnEquip() override;
	void End_BowString();
	
	void BeginPlay
	(
		class ACharacter* InOwner,
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		const TArray<FDoActionData>& InDoActionDatas,
		const TArray<FHitData>& InHitDatas
	) override;

	void Tick(float DeltaTime) override;

	virtual void DoAction() override;
	virtual void Begin_DoAction() override; //공격 시작 구간
	virtual void End_DoAction() override; //공격 완료 구간

private:
	UFUNCTION()
	void OnArrowHit(class AActor* InCauser, class ACharacter* InOtherCharacter);
	
	UFUNCTION()
	void OnArrowEndPlay(class ACArrow* InDestroyer);
private:
	class USkeletalMeshComponent* SkeletalMesh;
	class UPoseableMeshComponent* PoseableMesh;

private:
	TArray<class ACArrow*> Arrows;
private:
	FVector OriginLocation;
	const bool* bEquiped;
	bool bAttachedString = true;

private:
	float* Bending;
};



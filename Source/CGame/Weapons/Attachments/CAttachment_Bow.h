// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAttachment.h"
#include "CAttachment_Bow.generated.h"

UCLASS()
class CGAME_API ACAttachment_Bow : public ACAttachment
{
	GENERATED_BODY()

	//EditDefaultsOnly는 블프 클래스에서만 편집 가능
private:
	UPROPERTY(EditDefaultsOnly,Category = "View")
		FVector2D ViewPitchRange = FVector2D(-40,30);
	//x를 min으로 사용
	FVector2D OriginViewPitchRange;

	//SkeletalMesh를 캡쳐해서 본을 수정
private:
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(VisibleAnywhere)
	class UPoseableMeshComponent* PoseableMesh;

public:
	float* GetAnimInstance_Bending();

public:
	ACAttachment_Bow();

public:
	void OnBeginEquip_Implementation() override;
	void OnUnEquip_Implementation() override;

protected:
	void BeginPlay() override;

public:
	void Tick(float DeltaTime) override;
};

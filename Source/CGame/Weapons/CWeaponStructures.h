// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DamageEvents.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponStructures.generated.h"



USTRUCT()//uclass위에 있어야함
struct FEquipmentData
{
	GENERATED_BODY()

public:
	//장착 관리 구조체
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;
	UPROPERTY(EditAnywhere)
		float PlatRate = 1;
	UPROPERTY(EditAnywhere)
		bool bCanMove = true;
	UPROPERTY(EditAnywhere)
		bool bUseControlRotation = true;
};
/**
 * 
 */

USTRUCT()//uclass위에 있어야함
struct  FDoActionData
{
	GENERATED_BODY()

public:
	//장착 관리 구조체
	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage;
	UPROPERTY(EditAnywhere)
	float PlayRate = 1;
	UPROPERTY(EditAnywhere)
	bool bCanMove = true;
	UPROPERTY(EditAnywhere)
	bool bFixedCamera;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACGhostTrail> GhostTrailClass;

public:
	void DoAction(class ACharacter* InOwner);
	void Destroy_GhostTrail();

private:
	class ACGhostTrail* GhostTrail; //생성된 애 저장
};

USTRUCT()//uclass위에 있어야함
struct  FHitData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage;
	
	UPROPERTY(EditAnywhere)
	float PlayRate = 1;
	
	UPROPERTY(EditAnywhere)
	float Power;

	//100이면 1m
	UPROPERTY(EditAnywhere)
	float Launch = 100;
	
	UPROPERTY(EditAnywhere)
	float StopTime;

	UPROPERTY(EditAnywhere)
	class USoundWave* Sound;
	
	UPROPERTY(EditAnywhere)
	class UParticleSystem* Effect;
	UPROPERTY(EditAnywhere)
	FVector EffectLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere)
	FVector EffectScale = FVector::OneVector;

	public:
	void SendDamage(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther);
	void PlayHitStop(UWorld* InWorld);
	void PlaySoundWave(class ACharacter* InOwner);

	void PlayEffect(UWorld* InWorld, const FVector& InLocation); //문제점
	void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotator);
};

USTRUCT()
struct FActionDamageEvent : public FDamageEvent
{
	GENERATED_BODY()

public:
	FHitData* HitData;
};

UCLASS()
class CGAME_API UCWeaponStructures : public UObject
{
	GENERATED_BODY()
};

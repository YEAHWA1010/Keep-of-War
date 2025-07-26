// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CWeaponComponent.h"
#include "CAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CGAME_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	//읽기만 하고, 입력할 수 있도록 변수로 나타내는 EditAnywhere
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Animation")
	float Speed;

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Animation")
	float Direction;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Animation")
	bool bFalling;

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Animation")
	float Pitch;
	
protected:
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Animation")
	bool bBow_Aiming;

protected:
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Animation")
	EWeaponType WeaponType = EWeaponType::Max;
	
public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	class ACharacter* OwnerCharacter; //블루프린트를 소유한 애
	class UCWeaponComponent* Weapon;

private:
	UFUNCTION()
	void OnWeaponTypeChanged(EWeaponType InPrevType,EWeaponType InNewType);

private:
	FRotator PreRotation;
};

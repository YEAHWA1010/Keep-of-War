// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance_Bow.generated.h"

/**
 * 
 */
UCLASS()
class CGAME_API UCAnimInstance_Bow : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="Animation")
		float Bending;

public:
	FORCEINLINE float* GetBending() { return &Bending; }

	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

};

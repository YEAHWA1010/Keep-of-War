// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CHealthPointComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthPointChanged, float, InHealth, float, InMaxHealth);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CGAME_API UCHealthPointComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere,Category="Health")
	float MaxHealth = 100.0f;
public:	
	// Sets default values for this component's properties
	UCHealthPointComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void Damage(float InAmount);
	FORCEINLINE bool IsDead() {return Health <= 0.0f;}

	FORCEINLINE float GetHealth() { return Health; }
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }

private:
	float Health;

public:
	FHealthPointChanged OnHealthPointChanged;
};

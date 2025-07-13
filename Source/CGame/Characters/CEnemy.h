// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/IStateable.h"
#include "Components/CStateComponent.h"
#include "CEnemy.generated.h"

UCLASS()
class CGAME_API ACEnemy : public ACharacter ,public IIStateable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACEnemy();
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	void End_Damaged() override;

private:
	UPROPERTY(EditAnywhere,Category = "Color")
	FLinearColor OriginColor = FLinearColor::White;

	UPROPERTY(EditAnywhere,Category = "Montage")
	class UAnimMontage* DamagedMontage;

	UPROPERTY(EditAnywhere,Category = "Montage")
	class UAnimMontage* DeadMontage;

protected:
	//액터컴포넌트영역
	UPROPERTY(VisibleAnywhere)
	class UCHealthPointComponent* HealthPoint;

	UPROPERTY(VisibleAnywhere)
	class UCMovementComponent* Movement;

protected:
	UPROPERTY(VisibleAnywhere)
	class UCStateComponent* State;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//레퍼런스 + const => 복사 안됨
	//레퍼런스만 하면 수정 가능
	void Change_Color(const FLinearColor& InColor);

	UFUNCTION()
	void RestoreColor();

	UFUNCTION()
	void OnStateTypeChanged(EStateType InPrevType,EStateType InNewType);
	void Dead();
	virtual void End_Dead() override;
	
protected:
	virtual void Damaged();
	
private:
	FTimerHandle ChangeColor_TimerHandle;

private:
	struct FDamageData
	{
		float Power;
		class ACharacter* Attacker;
		class AActor* Causer;

		struct FActionDamageEvent* Event;
	} DamageData;
	
};



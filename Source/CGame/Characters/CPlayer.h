// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IStateable.h"
#include "Characters/IStateable.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CParkourComponent.h"
#include "Components/CWeaponComponent.h"
#include "GenericTeamAgentInterface.h"
#include "CPlayer.generated.h"


UCLASS()
class CGAME_API ACPlayer : public ACharacter , public IIStateable , public IGenericTeamAgentInterface
{
	GENERATED_BODY()

private:
	
	UPROPERTY(EditAnywhere,Category="Evade")
	class UAnimMontage* BackStepMontage; //몽타주 자료형

	UPROPERTY(EditDefaultsOnly,Category="Team")
	uint8 TeamID = 1; //몽타주 자료형

	UPROPERTY(EditAnywhere, Category = "UserInterface")
	TSubclassOf<class UCUserWidget_Player> UIClass;

//상태를 리턴만 해주는 애
// public:
// 	FORCEINLINE bool GetEquipped() {return bEquipped;}
	
private:
	UPROPERTY(VisibleAnywhere)
		class UCWeaponComponent* Weapon;
	//씬컴포넌트 영역
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		class USceneComponent* ArrowGroup;

	UPROPERTY(VisibleAnywhere)
		class UArrowComponent* Arrows[(int32)EParkourArrowType::Max];

private:
	//액터컴포넌트영역
	UPROPERTY(VisibleAnywhere)
		class UCMovementComponent* Movement;

	UPROPERTY(VisibleAnywhere)
	class UCTargetComponent* Target;
	
	UPROPERTY(VisibleAnywhere)
	class UCStateComponent* State;

	UPROPERTY(VisibleAnywhere)
	class UCParkourComponent* Parkour;
	
	UPROPERTY(VisibleAnywhere)
	class UCHealthPointComponent* HealthPoint;
	
public:
	// Sets default values for this character's properties
	ACPlayer();
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void OnEvade();

	FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(TeamID); }

//
private:
// 	//파쿠르 실행키
 	void OnSubAction();
	void OffSubAction();
//
public:
 	void Landed(const FHitResult& Hit) override;
//
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType,EStateType InNewType);
	UFUNCTION()
	void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);

	void End_BackStep() override;

private:
	class UCUserWidget_Player* UserInterface;
	
	UPROPERTY(EditAnywhere,Category = "Montage")
	class UAnimMontage* DamagedMontage;

	void Damaged();
	
private:
	struct FDamageData
	{
		float Power;
		class ACharacter* Attacker;
		class AActor* Causer;

		struct FActionDamageEvent* Event;
	} DamageData;
};




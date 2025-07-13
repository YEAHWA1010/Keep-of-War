// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CTargetComponent.generated.h"


UCLASS()
class CGAME_API UCTargetComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere,Category="Debug")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

	UPROPERTY(EditAnywhere,Category="Setting")
		class UParticleSystem* ParticleAsset;
	
	UPROPERTY(EditAnywhere,Category="Settings")
		float TraceDistance = 1000.0f;

	UPROPERTY(EditAnywhere,Category="Settings")
	float FinishAngle = 2.0f; 

	//값이 클수록 보간이 빠름
	UPROPERTY(EditAnywhere,Category="Settings")
	float InterSpeed = 5.0f;
	
public:	
	// Sets default values for this component's properties
	UCTargetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	void Toggle();
	void MoveLeft(); //왼쪽 타깃으로 이동
	void MoveRight();
private:
	void Start();
	void End();
	void Move(bool bRight); //true = 오른쪽
private:
	void Change(class ACharacter* InCandidate);

private:
	class ACharacter* GetNearlyFrontAngle(const TArray<FHitResult>& InHitResults);
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	class ACharacter* OwnerCharacter; // 컴포넌트의 소유자
	class ACharacter* Target; //타겟된 애
	class UParticleSystemComponent* Particle;

private:
	bool bMovingFocus;
};

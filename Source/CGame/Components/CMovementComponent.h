// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CMovementComponent.generated.h"

UENUM(BlueprintType)
enum class ESpeedType : uint8 //uint8
{
	Walk = 0, Run , Sprint, Max,
};

UCLASS()
//UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CGAME_API UCMovementComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere,Category="CameraSpeed")
	float HorizontalLook = 45;

	UPROPERTY(EditAnywhere,Category="CameraSpeed")
	float VerticalLook = 45;

private:
	UPROPERTY(EditAnywhere,Category="Speed")
	float Speed[(int32)ESpeedType::Max] = { 200,400,600};
	
public:	
	// Sets default values for this component's properties
	UCMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	//함수로 스피드 설정
	void SetSpeed(ESpeedType Intype);

public:
	void OnSprint();
	void OnRun();
	void OnWalk();
	void Move();
	void Stop();

	FORCEINLINE bool CanMove(){return bCanMove;}
	FORCEINLINE bool GetFixedCamera() {return bFixedCamera;}
	FORCEINLINE void EnableFixedCamera() {bFixedCamera = true;}
	FORCEINLINE void DisableFixedCamera() {bFixedCamera = false;}
	
	FORCEINLINE float GetWalkSpeed() { return Speed[(int32)ESpeedType::Walk]; }
	FORCEINLINE float GetRunSpeed() { return Speed[(int32)ESpeedType::Run]; }
	FORCEINLINE float GetSprintSpeed() { return Speed[(int32)ESpeedType::Sprint]; }

public:
	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);

	//카메라의 회전을 따르겠다
	void EnableControlRotation();
	//이동하는 방향을 따르겠다
	void DisableControlRotation();

private:
	class ACharacter* OwnerCharacter;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	bool bCanMove = true; //움직일 수 있는가
	bool bFixedCamera;
		
};



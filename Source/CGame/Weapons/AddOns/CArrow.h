// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CArrow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FArrowHit, class AActor*, InCauser , class ACharacter*, InOtherCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProjectileEndPlay, class ACArrow*, InDestroyer);

UCLASS()
class CGAME_API ACArrow : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* Projectile;

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere , Category = "LifeSpan")
	float LifeSpanAfterCollision = 1;
	
public:	
	// Sets default values for this actor's properties
	ACArrow();
	void Shoot(const FVector& InForward);
public:
	FORCEINLINE void AddIgonreActor(AActor* InActor){Ignores.Add(InActor);}

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	// 이 목록에 있는 애들은 데미지 안줌
	TArray<AActor*> Ignores;
private:
	UFUNCTION()
	void OnComponentHit(
	UPrimitiveComponent* HitComp,       // 충돌한 내 컴포넌트
	AActor* OtherActor,                 // 충돌한 상대 액터
	UPrimitiveComponent* OtherComp,     // 충돌한 상대 컴포넌트
	FVector NormalImpulse,              // 충돌 시 가해진 물리 힘 (Impulse)
	const FHitResult& Hit               // 충돌 상세 정보 (위치, 노멀 등)
);

public:
	FArrowHit OnHit; //델리게이트 이름
	FProjectileEndPlay OnEndPlay;
};

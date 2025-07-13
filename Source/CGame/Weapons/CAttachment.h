// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"

//콜리전
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentBeginCollision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentEndCollision);

//데미지
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentBeginOverlap, class ACharacter*, InAttacker, AActor*, InAttachCauser, class ACharacter*, InOther);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttachmentEndOverlap, class ACharacter*, InAttacker, class ACharacter*, InOther);

UCLASS()
class CGAME_API ACAttachment : public AActor
{
	GENERATED_BODY()
	//블루프린트에 접근하려고 protected
protected:
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
		class USceneComponent* Root;
public:	
	// Sets default values for this actor's properties
	ACAttachment();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void OnCollisions();
	void OffCollisions();

public:
	UFUNCTION(BlueprintNativeEvent)//블프에서 재정의
	void OnBeginEquip();
	virtual void OnBeginEquip_Implementation(){}; //상속받은 c에서 재정의

	UFUNCTION(BlueprintNativeEvent)
	void OnUnEquip();
	virtual void OnUnEquip_Implementation(){};

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	

public:
	FAttachmentBeginCollision OnFAttachmentBeginCollision;
	FAttachmentBeginCollision OnFAttachmentEndCollision;
	
	FAttachmentBeginOverlap OnAttachmentBeginOverlap;
	FAttachmentEndOverlap OnAttachmentEndOverlap;

protected:
	UPROPERTY(BlueprintReadOnly,Category = "Game")
		class ACharacter* OwnerCharacter;

protected:
	//충돌체 몇개일지 모르니까 배열로 처리
	TArray<class UShapeComponent*> Collisions;

	//자식에서(블프에서만 접근)
	UFUNCTION(BlueprintCallable)
		void AttachTo(FName InSocketname);
	
	UFUNCTION(BlueprintCallable, Category = "Attach")
	void AttachToCollision(FName InCollisionName);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

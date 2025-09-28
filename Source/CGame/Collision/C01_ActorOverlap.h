// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CGame.h"
#include "GameFramework/Actor.h"
#include "C01_ActorOverlap.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTriggeredDelegate);

UCLASS()
class CGAME_API AC01_ActorOverlap : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Box;
	
public:	
	AC01_ActorOverlap();
	
	UPROPERTY(BlueprintAssignable)
	FOnTriggeredDelegate OnTriggered; 

protected:
	virtual void BeginPlay() override;
	

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	

};

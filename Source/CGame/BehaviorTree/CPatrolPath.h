// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPatrolPath.generated.h"

UCLASS()
class CGAME_API ACPatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPatrolPath();
	void OnConstruction(const FTransform& Transform) override;

private:
	UPROPERTY(EditAnywhere,Category="Path")
	bool bLoop;

	UPROPERTY(EditAnywhere,Category="Path")
	int Index;

	UPROPERTY(EditAnywhere,Category="Path")
	bool bReverse;

private:
	UPROPERTY(VisibleDefaultsOnly)
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	class USplineComponent* Spline;

	UPROPERTY(VisibleAnywhere)
	class UTextRenderComponent* Text;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	FORCEINLINE class USplineComponent* GetSpline() { return Spline; }
public:
	FVector GetMoveTo();
	void UpdateIndex();
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

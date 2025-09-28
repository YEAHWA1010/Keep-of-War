// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CGame.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "InteractDoor.generated.h"

UCLASS()
class CGAME_API AInteractDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractDoor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DoorMesh;

	UFUNCTION()
	void HandleTriggered();

	UFUNCTION()
	void OpenDoor(float Value);

	

protected:
	
	FTimeline TimeLine;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* DoorSound;

	UPROPERTY(EditAnywhere)
	UCurveFloat* CurveFloat;

private:
	bool IsOpen = false;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGhostTrail.generated.h"

UCLASS()
class CGAME_API ACGhostTrail : public AActor
{
	GENERATED_BODY()

	//게임상에서는 생성을 시켜서 사용 예정
private:
	UPROPERTY(EditDefaultsOnly , Category = "Capture")
	//몇초뒤에 시작될지
		float StartDelay = 0;
	//잔상 캡쳐할 간격
	UPROPERTY(EditDefaultsOnly , Category = "Capture")
		float Interval = 0.25f;
	//잔상 색상
	UPROPERTY(EditDefaultsOnly , Category = "Capture")
		FLinearColor Color = FLinearColor(1,1,1,1);
	UPROPERTY(EditDefaultsOnly , Category = "Capture")
		float Exponent = 1;
	//잔상을 캐릭터 대비 몇으로 할거냐
	UPROPERTY(EditDefaultsOnly , Category = "Capture")
		FVector Scale = FVector::OneVector;
	//스케일에 조정할 곱하기 값
	UPROPERTY(EditDefaultsOnly , Category = "Capture")
		FVector ScaleAmount = FVector::ZeroVector;
	UPROPERTY(VisibleAnywhere)
		class UPoseableMeshComponent* Mesh;

public:	
	// Sets default values for this actor's properties
	ACGhostTrail();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//게임이 종료되거나 삭제되었을 때 호출됨
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	class ACharacter* OwnerCharacter;
	class UMaterialInstanceDynamic* Material;

	FTimerHandle TimerHandle;

};

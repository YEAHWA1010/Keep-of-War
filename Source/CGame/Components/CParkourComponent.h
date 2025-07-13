// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/DataTable.h"
//#include "Kismet/KismetSystemLibrary.h"
#include "CParkourComponent.generated.h"

UENUM(BlueprintType)
enum class EParkourArrowType : uint8
{
	Center = 0,Head,Foot,Left,Right,Land,Max,
};

UENUM(BlueprintType)
enum class EParkourType : uint8
{
	Climb = 0,Fall,Slide,Short,Normal,Wall,Max
};

//외부 에셋으로 불러오는 경우 UStruct 붙이기
//UCLASS 위에 존재해야함 BlueprintType 블프코드에 공개된다는 뜻
//전방선언 필요 x
USTRUCT(BlueprintType)
struct FParkrourData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EParkourType Type;
	UPROPERTY(EditAnywhere)
		UAnimMontage* Montage;
	UPROPERTY(EditAnywhere)
		float PlayRate = 1; //몽타주 플레이할 속도
	UPROPERTY(EditAnywhere)
		FName SectionName;
	UPROPERTY(EditAnywhere) //수행 최소 거리
		float MinDistance;	//약간 거리를 두고 뛰어 넘기 위해
	UPROPERTY(EditAnywhere) //수행 최소 거리
		float MaxDistance;	//약간 거리를 두고 뛰어 넘기 위해
	UPROPERTY(EditAnywhere) //수행 최대 거리
		float Extent;
	UPROPERTY(EditAnywhere) //카메라 회전 허용 여부
		bool bFixedCamera;


public:
	void PlayMontage(class ACharacter* InCharacter);
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CGAME_API UCParkourComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere,Category="Data")
	UDataTable* DataTable;
private:
	UPROPERTY(EditAnywhere,Category = "Temp");
	FParkrourData TestData;

private:
	UPROPERTY(EditAnywhere , Category="Trace")
	float TraceDistance = 600;
	UPROPERTY(EditAnywhere, Category="Trace")
	float AvailableFrontAngle = 15;
	
	UPROPERTY(EditAnywhere , Category="Trace")
	TEnumAsByte<EDrawDebugTrace::Type> DebugType;
	//선을 쏠 거리
	//enum 자료형 안정해진 애들은 TEnumAsByte를 사용해야한다
public:	
	// Sets default values for this component's properties
	UCParkourComponent();
public:
	void DoParkour(bool bLanded = false);
	void End_DoParkour();
	void End_DoParkour_Climb();

	bool Check_FallMode();
	void DoParkour_Fall();
	void End_DoParkour_Fall();

	bool Check_SlideMode();
	void DoParkour_Slide();
	void End_DoParkour_Slide();

	bool Check_ObstacleMode(EParkourType InType , FParkrourData& OutData);
	void DoParkour_Obstacle(EParkourType InType , FParkrourData& InData);
	void End_DoParkour_Obstacle();
	
private:
	TMap<EParkourType,TArray<FParkrourData>> DataMap;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	EParkourType Type = EParkourType::Max;
	class ACharacter* OwnerCharacter;
	class UArrowComponent* Arrows[(int32)EParkourArrowType::Max];
	FHitResult HitResults[(int32)EParkourArrowType::Max];

private:
	//어떤 파쿠르를 수행할지 추적
	void CheckTrace_Center();
	void CheckTrace_Head();
	void CheckTrace_Foot();
	void CheckTrace_Side(); //좌우
	void CheckTrace_Land(); //지면
	void LineTrace(EParkourArrowType InType);
	

private:
	bool Check_Obstacle();
	bool Check_ClimbMode();
	void DoParkour_Climb();
	
private:
	AActor* HitObstacle;
	FVector HitObstacleExtent;
	float HitDistance;
	float ToFrontYaw;
	bool bFirstFalling;
	AActor* BackupObstacle;

};






// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CParkourComponent.h"

#include "AsyncTreeDifferences.h"
#include "Global.h"
#include "InputBehavior.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CMovementComponent.h"
#include "UObject/PropertyTempVal.h"

//#define LOG_UCParkourComponent

void FParkrourData::PlayMontage(class ACharacter* InCharacter)
{
	if (bFixedCamera)
	{
		UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InCharacter);

		if (!!movement)
		{
			movement->EnableFixedCamera();
		}
			
	}
	InCharacter->PlayAnimMontage(Montage,PlayRate,SectionName);	
}

//-----------------------------------------------------------------------------

// Sets default values for this component's properties
UCParkourComponent::UCParkourComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	CHelpers::GetAsset<UAnimMontage>(&TestData.Montage,"AnimMontage'/Game/Characters/Montages/Parkour/Run_Climb_Montage.Run_Climb_Montage'");
	CHelpers::GetAsset<UDataTable>(&DataTable,"/Script/Engine.DataTable'/Game/Parkour/DT_Parkour.DT_Parkour'");
}

void UCParkourComponent::DoParkour(bool bLanded)
{
	//파쿠르 수행중인지 체크
	CheckFalse(Type == EParkourType::Max);

	if (bLanded && Check_FallMode())
	{
		DoParkour_Fall();
		return;
	}

	HitObstacle = nullptr;
	HitObstacleExtent = FVector::ZeroVector;
	HitDistance = 0.0f;
	ToFrontYaw = 0.0f;

	CheckTrace_Center();

	if (!!HitObstacle)
	{
		//머리 쪽에 닿는게 있는지 체크
		CheckTrace_Head();
		CheckTrace_Foot();
		CheckTrace_Side();
	}

	CheckFalse(Check_Obstacle());
	
	//기어갈 수 있는지 체크
	if (Check_ClimbMode())
	{
		DoParkour_Climb();
		return;
	}

	if (Check_SlideMode())
	{
		DoParkour_Slide();
		
		return;
	}

	FParkrourData data;
	if (Check_ObstacleMode(EParkourType::Normal,data))
	{
		//노멀에 대해서만 체크
		DoParkour_Obstacle(EParkourType::Normal,data);
		
		return;
	}

	if (Check_ObstacleMode(EParkourType::Short,data))
	{
		//노멀에 대해서만 체크
		DoParkour_Obstacle(EParkourType::Short,data);
		
		return;
	}
	
	if (Check_ObstacleMode(EParkourType::Wall,data))
	{
		//노멀에 대해서만 체크
		DoParkour_Obstacle(EParkourType::Wall,data);
		
		return;
	}
	
}

void UCParkourComponent::End_DoParkour()
{	
	switch (Type)
	{	
		case EParkourType::Climb:
			End_DoParkour_Climb();
			break;

		case EParkourType::Fall:
			End_DoParkour_Fall();
			break;

		case EParkourType::Slide:
			End_DoParkour_Slide();
			break;
		
		
		case EParkourType::Short:
		case EParkourType::Normal:
		case EParkourType::Wall:
			End_DoParkour_Obstacle();
			break;	
	}

	Type = EParkourType::Max;

	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);

	if (!!movement)
	{
		if (movement->GetFixedCamera() == true)
		{
			movement->DisableFixedCamera();	
		}
	}
}



bool UCParkourComponent::Check_FallMode()
{
	//false면 뛴 상태가 아니므로 끝내기
	CheckFalseResult(bFirstFalling,false);
	//뛴상태가 되었으므로 , 최초 낙하는 아님
	bFirstFalling = false;
	
	float distance = HitResults[(int32)EParkourArrowType::Land].Distance; //높이
	
	const TArray<FParkrourData>* datas = DataMap.Find(EParkourType::Fall);
	//(*datas)[0]data가 객체가 되어서 배열에 넘어오고 걔의 0번째
	//(*datas)[0].MinDistance
	//히트된 거리의 최소거리보다 작으면 하면 안됨
	//700안에 있으면 구르기 안함
	CheckFalseResult((*datas)[0].MinDistance < distance,false);
	CheckFalseResult((*datas)[0].MaxDistance > distance,false);
	
	return true;
}

void UCParkourComponent::DoParkour_Fall()
{
	Type = EParkourType::Fall;

	(*DataMap.Find(EParkourType::Fall))[0].PlayMontage(OwnerCharacter);
}

void UCParkourComponent::End_DoParkour_Fall()
{
	
}



// Called when the game starts
void UCParkourComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<FParkrourData*> rows;
	//전체 행 가져오기
	DataTable->GetAllRows<FParkrourData>("",rows);

	for (int32 i = 0; i < (int32)EParkourType::Max; i++)
	{
		TArray<FParkrourData> temp;
		for (FParkrourData* data : rows)
		{
			if (data -> Type == (EParkourType)i)
			{
				temp.Add(*data); //포인터 기호로 data객체 자체를 줌
			}
		}

		DataMap.Add((EParkourType)i,temp);
	}

#ifdef LOG_UCParkourComponent
	//키와 값을 묶어주는 역할 , foreach로 DataMap 돌림
	for (TPair<EParkourType,TArray<FParkrourData>> temp : DataMap)
	{
		CLog::Log(StaticEnum<EParkourType>()->GetNameStringByValue((uint8)temp.Key));
			
		for (FParkrourData tempData : temp.Value)
		{
			//타입이랑 몽타주 출력 로그
			CLog::Log(tempData.Montage->GetPathName());
		}
	}
#endif
	
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	
	USceneComponent* arrows = CHelpers::GetComponent<USceneComponent>(OwnerCharacter,"Arrows"); //찾아오려는 이름
	TArray<USceneComponent*> components;
	arrows->GetChildrenComponents(false,components);

	for (int32 i = 0; i < (int32)EParkourArrowType::Max; i++)
	{
		//CLog::Log(components[i]->GetName());
		Arrows[i] = Cast<UArrowComponent>(components[i]);
	}
}

void UCParkourComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckTrace_Land();
}

void UCParkourComponent::CheckTrace_Center()
{
	EParkourArrowType type = EParkourArrowType::Center;
	LineTrace(type);

	const FHitResult& hitResult = HitResults[(int32)type];
	CheckFalse(hitResult.bBlockingHit);
	//파쿠르 블럭 반응에 막힌애만 파쿠르 실행
	UStaticMeshComponent* mesh = CHelpers::GetComponent<UStaticMeshComponent>(hitResult.GetActor());
	CheckNull(mesh);

	HitObstacle = hitResult.GetActor();

	FVector minBound,maxBound;
	
	mesh->GetLocalBounds(minBound,maxBound);
	float x = FMath::Abs(minBound.X - maxBound.X);
	float y = FMath::Abs(minBound.Y - maxBound.Y);
	float z = FMath::Abs(minBound.Z - maxBound.Z);

	HitObstacleExtent = FVector(x,y,z);
	HitDistance = hitResult.Distance; //충돌 거리

	//플레이어가 들어가는 방향의 회전값
	//impactNormal의 안쪽방향
	ToFrontYaw = UKismetMathLibrary::MakeRotFromX(-hitResult.ImpactNormal).Yaw;
	
	CLog::Print(HitObstacleExtent);
	CLog::Print(HitDistance);

#ifdef LOG_UCParkourComponent
	//FString::SanitizeFloat(HitDistance) float -> 문자열로
	CLog::Print("HitObstacle : " + HitObstacle -> GetName() ,1100);
	CLog::Print("HitObstacleExtent : " + HitObstacleExtent.ToString(),1101);
	CLog::Print("HitDistance : " + FString::SanitizeFloat(HitDistance),1102);
	CLog::Print("HitDistance : " + FString::SanitizeFloat(ToFrontYaw),1103);	
#endif
}

void UCParkourComponent::CheckTrace_Head()
{
	LineTrace(EParkourArrowType::Head);
}

void UCParkourComponent::CheckTrace_Foot()
{
	LineTrace(EParkourArrowType::Foot);
}

void UCParkourComponent::CheckTrace_Side()
{
	LineTrace(EParkourArrowType::Left);
	LineTrace(EParkourArrowType::Right);
}

void UCParkourComponent::CheckTrace_Land()
{
	//IsFalling이 아닌 경우 밑에 실행x
	//추락이 시작되면 true
	CheckFalse(OwnerCharacter->GetCharacterMovement()->IsFalling());
	//이미 true이면 아래 실행x
	CheckTrue(bFirstFalling);
	bFirstFalling = true;

	UArrowComponent* arrow = Arrows[(int32)EParkourArrowType::Land];

	FLinearColor color = FLinearColor(arrow->ArrowColor);

	//GetComponentToWorld()는 월드트랜스폼으로 리턴해줌
	FTransform transform = arrow->GetComponentToWorld();

	FVector start = transform.GetLocation();

	const TArray<FParkrourData> *datas = DataMap.Find(EParkourType::Fall);
	//회전된 방향의 전방벡터
	FVector end = start + transform.GetRotation().GetForwardVector() * (*datas)[0].Extent; //2000만큼의 길이의 선을 쏨

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);
	
	UKismetSystemLibrary::LineTraceSingle(GetWorld(),start,end,ETraceTypeQuery::TraceTypeQuery1,false,ignores,DebugType,HitResults[(int32)EParkourArrowType::Land],true,color,FLinearColor::White);
}

void UCParkourComponent::LineTrace(EParkourArrowType InType)
{
	UArrowComponent* arrow = Arrows[(int32)InType];
	FLinearColor color = FLinearColor(arrow->ArrowColor);

	//GetComponentToWorld()는 월드트랜스폼으로 리턴해줌
	FTransform transform = arrow->GetComponentToWorld();

	FVector start = transform.GetLocation();
	FVector end = start + OwnerCharacter->GetActorForwardVector() * TraceDistance;

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);
	
	UKismetSystemLibrary::LineTraceSingle(GetWorld(),start,end,ETraceTypeQuery::TraceTypeQuery3,false,ignores,DebugType,HitResults[(int32)InType],true,color,FLinearColor::White);
}

bool UCParkourComponent::Check_Obstacle()
{
	//HitObstacle이 없으면 false를 반환
	CheckNullResult(HitObstacle,false);

	bool b = true;
	b &= HitResults[(int32)EParkourArrowType::Center].bBlockingHit;
	b &= HitResults[(int32)EParkourArrowType::Left].bBlockingHit;
	b &= HitResults[(int32)EParkourArrowType::Right].bBlockingHit;
	CheckFalseResult(b,false); //b가 false면 넘어감

	FVector center = HitResults[(int32)EParkourArrowType::Center].ImpactNormal;
	FVector left = HitResults[(int32)EParkourArrowType::Left].ImpactNormal;
	FVector right = HitResults[(int32)EParkourArrowType::Right].ImpactNormal;

	//Equals는 오차값 내이면 같은값이라고 간주함
	//같은 경우 true로 패스함
	CheckFalseResult(center.Equals(left),false);
	CheckFalseResult(center.Equals(right),false);

	FVector start = HitResults[(int32)EParkourArrowType::Center].ImpactPoint; //충돌 지점
	FVector end = OwnerCharacter->GetActorLocation();
	//FindLookAtRotation() 현재에서 타깃을 바라보는 회전값 반환
	//부딪힌 곳의 플레이어를 바라보는 방향 , 수직벡터의 값
	float lookAt = UKismetMathLibrary::FindLookAtRotation(start,end).Yaw;

	FVector impactNormal = HitResults[(int32)EParkourArrowType::Center].ImpactNormal;
	float impactAt = UKismetMathLibrary::MakeRotFromX(impactNormal).Yaw;
	//부딪힌 곳의 플레이어를 바라보는 방향 , 수직벡터의 값
	float yaw = FMath::Abs(FMath::Abs(lookAt)-FMath::Abs(impactAt));
	CheckFalseResult(yaw <= AvailableFrontAngle,false);
	
#ifdef LOG_UCParkourComponent
	CLog::Print("lookAt : " + FString::SanitizeFloat(lookAt),1110);
	CLog::Print("impactNormal : " + impactNormal.ToString(),1111);
	CLog::Print("impactAt : " + FString::SanitizeFloat(impactAt),1112);
#endif

	return true;
}

bool UCParkourComponent::Check_ClimbMode()
{
	//머리가 닿지 않으면 false 리턴
	CheckFalseResult(HitResults[(int32)EParkourArrowType::Head].bBlockingHit,false);

	//데이터 불러오기
	DataMap[EParkourType::Climb];
	const TArray<FParkrourData>* datas = DataMap.Find(EParkourType::Climb);
	//(*datas)[0]data가 객체가 되어서 배열에 넘어오고 걔의 0번째
	//(*datas)[0].MinDistance
	//히트된 거리의 최소거리보다 작으면 하면 안됨
	CheckFalseResult((*datas)[0].MinDistance < HitDistance,false);
	CheckFalseResult((*datas)[0].MaxDistance > HitDistance,false);
	// //주언진 오차값 이하면 같은값으로 간주
	// //HitObstacleExtent.Z이 10차이보다 크면 false
	CheckFalseResult(FMath::IsNearlyEqual((*datas)[0].Extent,HitObstacleExtent.Z,10),false);
	
	return true;
}

void UCParkourComponent::DoParkour_Climb()
{
	Type = EParkourType::Climb;

	//센터 가운데를 쐈을때 닿은 지점으로 캐릭터를 움직여준다
	OwnerCharacter->SetActorLocation(HitResults[(int32)EParkourArrowType::Center].ImpactPoint);
	OwnerCharacter->SetActorRotation(FRotator(0,ToFrontYaw,0));
	(*DataMap.Find(EParkourType::Climb))[0].PlayMontage(OwnerCharacter);

	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}

void UCParkourComponent::End_DoParkour_Climb()
{
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

bool UCParkourComponent::Check_SlideMode()
{
	//발이 닿으면 안됨
	CheckTrueResult(HitResults[(int32)EParkourArrowType::Foot].bBlockingHit,false);

	//데이터 불러오기
	DataMap[EParkourType::Climb];
	const TArray<FParkrourData>* datas = DataMap.Find(EParkourType::Slide);
	//(*datas)[0]data가 객체가 되어서 배열에 넘어오고 걔의 0번째
	//(*datas)[0].MinDistance
	//히트된 거리의 최소거리보다 작으면 하면 안됨
	CheckFalseResult((*datas)[0].MinDistance < HitDistance,false);
	CheckFalseResult((*datas)[0].MaxDistance > HitDistance,false);

	UArrowComponent* arrow = Arrows[(int32)EParkourArrowType::Foot];
	FLinearColor color = FLinearColor(arrow->ArrowColor);

	FTransform transform = arrow->GetComponentToWorld(); //밑바닥보다 살짝 위
	FVector arrowLocation = transform.GetLocation();
	FVector ownerLocation = OwnerCharacter->GetActorLocation();

	float extent = (*datas)[0].Extent; //데이터의 extent ex)30
	float z = arrowLocation.Z + extent;

	FVector forward = OwnerCharacter->GetActorForwardVector();
	//방향만 만들거니까 높이는 빼기
	forward = FVector(forward.X,forward.Y,0);

	FVector start = FVector(arrowLocation.X,arrowLocation.Y,z); //위
	FVector end = start + forward * TraceDistance; //전방방향으로 추적 거리만큼

	TArray<AActor*> ignores;
	FHitResult hitResult;
	//x방향은 TraceDistance이므로 필요없고 가로부피,세로부피만 필요 / y는 좌우 z는 높이 , GetActorRotation이 방향 start , end는 길이를 말함
	UKismetSystemLibrary::BoxTraceSingle(GetWorld(),start,end,FVector(0,extent,extent),OwnerCharacter->GetActorRotation(),TraceTypeQuery1,false,ignores,DebugType,hitResult,true,color,FLinearColor::White);
	//bBlockingHit이 true면 하면 안됨
	CheckTrueResult(hitResult.bBlockingHit,false);
	return true;
}

void UCParkourComponent::DoParkour_Slide()
{
	Type = EParkourType::Slide;
	
	//틀어진 방향 보정해서 정면으로 들어가도록 함
	OwnerCharacter->SetActorRotation(FRotator(0,ToFrontYaw,0));
	(*DataMap.Find(EParkourType::Slide))[0].PlayMontage(OwnerCharacter);

	BackupObstacle = HitObstacle;
	BackupObstacle->SetActorEnableCollision(false);
}

void UCParkourComponent::End_DoParkour_Slide()
{
	BackupObstacle->SetActorEnableCollision(true);
	BackupObstacle = nullptr;
}

bool UCParkourComponent::Check_ObstacleMode(EParkourType InType, FParkrourData& OutData)
{
	//머리가 닿으면 안됨
	CheckTrueResult(HitResults[(int32)EParkourArrowType::Head].bBlockingHit,false);

	const TArray<FParkrourData>* datas = DataMap.Find(InType);
	
	for (int32 i = 0; i < (*datas).Num(); i++)
	{
		bool b = true;
		b &= (*datas)[i].MinDistance < HitDistance;
		b &= (*datas)[i].MaxDistance > HitDistance;
		b &= FMath::IsNearlyEqual((*datas)[i].Extent,HitObstacleExtent.Y,10);

		//현재 for문 돌리는 데이터의 번호를 넣어줌
		OutData = (*datas)[i];
		//b가 true면 끝
		CheckTrueResult(b,true);
	}
	//위에서 하나라도 만족하면 true
	return false;
}

void UCParkourComponent::DoParkour_Obstacle(EParkourType InType, FParkrourData& InData)
{
	Type = InType;
	
	//틀어진 방향 보정해서 정면으로 들어가도록 함
	OwnerCharacter->SetActorRotation(FRotator(0,ToFrontYaw,0));
	InData.PlayMontage(OwnerCharacter);

	BackupObstacle = HitObstacle;
	BackupObstacle->SetActorEnableCollision(false);
}

void UCParkourComponent::End_DoParkour_Obstacle()
{
	BackupObstacle->SetActorEnableCollision(true);
	BackupObstacle = nullptr;
}

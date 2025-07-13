// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CTargetComponent.h"
#include "Global.h"
#include "ParticleHelper.h"
#include "Characters/CPlayer.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/CStateComponent.h"

// Sets default values for this component's properties
UCTargetComponent::UCTargetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CHelpers::GetAsset<UParticleSystem>(&ParticleAsset,"ParticleSystem'/Game/AdvancedMagicFX12/particles/P_ky_aura_shine.P_ky_aura_shine'");
	// ...
}


// Called when the game starts
void UCTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	// ...
	
}

void UCTargetComponent::Toggle()
{
	if (Target == nullptr)
	{
		Start();
		return;
	}

	End();
}

void UCTargetComponent::Start()
{
	FVector Location = OwnerCharacter->GetActorLocation();

	TArray<AActor*> Ignores;
	Ignores.Add(OwnerCharacter);

	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(),Location,Location,TraceDistance,"Targeting",false,Ignores,DrawDebug,HitResults,true);

	//가장 정면에 가까운애한테 선
	//DrawDebugLine(GetWorld(),Location,Candidate->GetActorLocation(),FColor::Red,true,5);

	ACharacter* Candidate = GetNearlyFrontAngle(HitResults);
	Change(Candidate);
}

void UCTargetComponent::End()
{
	Target = nullptr;

	if (!!Particle)
	{
		Particle ->DestroyComponent();
	}
}

void UCTargetComponent::MoveLeft()
{
	Move(false);
}

void UCTargetComponent::MoveRight()
{
	Move(true);
}

void UCTargetComponent::Move(bool bRight)
{
	//포커스 이동 중이면 눌러도 반응 x
	CheckTrue(bMovingFocus);//포커스 이동 중이라면 Move 수행 x
	
	FVector Location = OwnerCharacter->GetActorLocation();
	
	TArray<AActor*> Ignores;
	Ignores.Add(OwnerCharacter);
	Ignores.Add(Target); //현재 타깃은 필요없음
	
	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(),Location,Location,TraceDistance,"Targeting",false,Ignores,DrawDebug,HitResults,true);

	TMap<float,ACharacter*> Map;
	
	for(const FHitResult& hitResult : HitResults) //복사 안되게
	{
		ACharacter* Character = Cast<ACharacter>(hitResult.GetActor());

		if(Character == nullptr)
		{
			continue;
		}

		//GetControlRotation()의 전방을 구하기 위해 FQuat
		FVector Forward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();

		FVector Location2 = Character->GetActorLocation();
		FVector Direction = Location2 - Location; //owner가 적을 바라보는 방향
		Direction.Normalize();

		FVector Cross = FVector::CrossProduct(Forward, Direction); //위로 솟구치는 방향
		//cross에서 중요한건 수직간 거리 => 이 거리가 가까워야 다음 타깃 , 거리값이 가장 작은애

		//0,0,1 => UpVector
		float Dot = FVector::DotProduct(FVector::UpVector, Cross);
		Map.Add(Dot,Character);
	}

	float Mininum = FLT_MAX;
	ACharacter* Candidate = nullptr;
	
	for (TPair<float,ACharacter*> Pair : Map)
	{
		float Key = Pair.Key; //외적한 거리
		
		if (bRight)
		{
			//오른쪽 대상
			if(Key < 0.0f) //절댓값을 써서 부호없애고 가장 작은 값 선택
				continue;
		}

		else
		{
			//왼쪽 대상
			if(Key > 0.0f)
				continue;
		}

		if (FMath::Abs(Key) < Mininum)
		{
			Mininum = FMath::Abs(Key);
			//*Map.Find(Key);
			Candidate = Pair.Value; //Map.Find(Key);이게 **Character 이거에 *는 *Character
		}
	}
		bMovingFocus = true;
		Change(Candidate);	
}

void UCTargetComponent::Change(class ACharacter* InCandidate)
{
	if (InCandidate == nullptr)
	{
		End();
		return;
	}

	if (!!Particle)
	{
		//이미 파티클이 설정되어있다면 원래꺼는 삭제해야함
		Particle ->DestroyComponent();
	}

	Particle = UGameplayStatics::SpawnEmitterAttached(ParticleAsset,InCandidate->GetMesh(),"Targeting",FVector::ZeroVector,FRotator::ZeroRotator,EAttachLocation::KeepRelativeOffset);

	Target = InCandidate;
}

class ACharacter* UCTargetComponent::GetNearlyFrontAngle(const TArray<FHitResult>& InHitResults)
{
	float Angle = -2.0f; //비교대상이 없을경우에 언리얼에서 -2.0으로 언리얼에서 예외처리
	//가장 큰값을 찾을려면 가장 작은값에서 시작
	//가장 작은 내적값은 언리얼에서 -2 =>

	ACharacter* Candidate = nullptr;
	Change(Candidate);
	
	for (int i = 0; i < InHitResults.Num(); i++)
	{
		FVector TargetLocation = InHitResults[i].GetActor()->GetActorLocation();
		FVector Direction = TargetLocation - OwnerCharacter->GetActorLocation();
		Direction.Normalize();

		//플레이어 카메라의 회전값
		FRotator Rotator = OwnerCharacter->GetControlRotation();
		FVector Forward = FQuat(Rotator).GetForwardVector();

		//DotProduct , Dot
		float Dot = FVector::DotProduct(Direction,Forward);

		// FString	str;
		// str.Append(InHitResults[i].GetActor()->GetActorLabel());
		// str.Append(" : ");
		// str.Append(FString::SanitizeFloat(Dot));
		//CLog::Log(str);

		if (Dot >= Angle)
		{
			Angle = Dot;
			Candidate = Cast<ACharacter>(InHitResults[i].GetActor());
		}
	}

	return Candidate; //가장 가까운 적 리턴
}

// Called every frame
void UCTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckNull(Target);

	//타겟이 죽었으면 하지 않기 위해 스테이트 가져옴
	UCStateComponent* State = CHelpers::GetComponent<UCStateComponent>(Target);
	CheckNull(State);
	CheckTrue(State->IsDeadMode());
	// ...

	float Distance = OwnerCharacter->GetDistanceTo(Target); //두 개의 거리차 , 거리에서 벗어나면 타게팅 해제 시스템
	if(Distance > TraceDistance)
	{
		End();
		return;
	}

	//바라보는곳
	FRotator ControlRotation = OwnerCharacter->GetControlRotation();
	//OwnerCharacter가 Target을 바라보도록 하는 값
	FRotator OwnerToTarget = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(),Target->GetActorLocation());
	OwnerToTarget.Pitch = ControlRotation.Pitch; //원래 카메라의 피치값 그대로 사용
	//회전에 대한 linear => slerp

	APlayerController* Controller = OwnerCharacter->GetController<APlayerController>();
	
	if (ControlRotation.Equals(OwnerToTarget,FinishAngle)) //값 차이가 2도안에 들어와있다면 종료(고정)
	{
		bMovingFocus = false;
		Controller->SetControlRotation(OwnerToTarget);

		return;
	}

	//OwnerToTarget 가려는 곳의 roll, yaw roll은 무조건 0 
	FRotator targetRotation = FRotator(ControlRotation.Pitch,OwnerToTarget.Yaw,OwnerToTarget.Roll);
	
	//현재값 , 갈려고하는가값 , 회전값을 리턴
	FRotator Result = UKismetMathLibrary::RInterpTo(ControlRotation,targetRotation,DeltaTime,InterSpeed);
	Controller->SetControlRotation(Result);
}


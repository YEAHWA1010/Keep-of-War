// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CWeaponStructures.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Characters/CGhostTrail.h"

void FDoActionData::DoAction(class ACharacter* InOwner)
{
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);
	if (!!movement)
	{
		if (bCanMove == false)
		{
			movement->Stop();
		}
	}

	if (!!Montage)
	{
		InOwner->PlayAnimMontage(Montage,PlayRate);
	}

	if (!!GhostTrailClass)
	{
		CLog::Print("GhostTrailClass");
		FVector Location = InOwner->GetActorLocation();
		CLog::Print(Location);
		//중심 - 절반빼면 발이 나옴
		Location.Z -= InOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		CLog::Print(Location);
		
		FActorSpawnParameters Params;
		Params.Owner = InOwner;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		FTransform transform; //스판 위치 생성
		transform.SetTranslation(Location);
		
		GhostTrail = InOwner->GetWorld()->SpawnActor<ACGhostTrail>(GhostTrailClass,transform,Params);
	}
}

void FDoActionData::Destroy_GhostTrail()
{
	if(!!GhostTrail)
	{
		GhostTrail->Destroy();
	}
}

void FHitData::PlayEffect(UWorld* InWorld, const FVector& InLocation)
{
	CheckNull(Effect);
	
	FTransform transform;
	transform.SetLocation(EffectLocation);
	transform.SetScale3D(EffectScale);
	transform.AddToTranslation(InLocation); //InLocation위치만큼 (플레이어의 위치) 만큼 더함

	UGameplayStatics::SpawnEmitterAtLocation(InWorld,Effect,transform);
}

void FHitData::PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotator)
{
	CheckNull(Effect);
	
	FTransform transform;
	//InRotator는 플레이어의 회전값
	//그냥 전방 100이 아니라 회전된 애의 전방 100으로 함
	transform.SetLocation(InLocation + InRotator.RotateVector(EffectLocation));
	transform.SetScale3D(EffectScale);

	UGameplayStatics::SpawnEmitterAtLocation(InWorld,Effect,transform);
}

///////////////////////////////////////////////////////////////////////////////

void FHitData::SendDamage(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther)
{
	FActionDamageEvent e;
	e.HitData = this;

	InOther->TakeDamage(Power,e,InAttacker->GetController(),InAttackCauser);
}

void FHitData::PlayHitStop(UWorld* InWorld)
{
	//float은 0의 근삿값으로 체크 => 0 가까우면 0이라고 판단
	CheckTrue(FMath::IsNearlyZero(StopTime));

	//GetCurrentLevel() => 현재 열려있는 레벨
	TArray<ACharacter*> characters;
	for(AActor* actor : InWorld->GetCurrentLevel()->Actors)
	{
		ACharacter* character = Cast<ACharacter>(actor);

		if (!!character)
		{
			character->CustomTimeDilation = 1e-3f;

			characters.Add(character);
		}
	}


	FTimerDelegate timerDelegate;
	//람다라는건 이 안에서 함수 하나 정의하는 것
	//= 대입으로 가져오는 경우 (주소가 다른 경우) , 레퍼런스의 경우 => &
	timerDelegate.BindLambda([=]() //=(=캡쳐절) 이걸 안쓰면 외부에 있는걸 캡쳐해오지 못했다고해서 못씀
	{
		for (ACharacter* character : characters)
		{
			if (!!character) //사망한 경우
			{
				character->CustomTimeDilation = 1.0f;
			}
		}
	});
	
	//지역변수로 쓰는 경우 => 사용하고 없앰
	FTimerHandle timerHandle;
	InWorld->GetTimerManager().SetTimer(timerHandle,timerDelegate,StopTime,false);
}

void FHitData::PlaySoundWave(class ACharacter* InOwner)
{
	CheckNull(Sound);
	UWorld* World = InOwner->GetWorld();
	FVector Location = InOwner->GetActorLocation();
	
	UGameplayStatics::SpawnSoundAtLocation(World,Sound, Location);
}

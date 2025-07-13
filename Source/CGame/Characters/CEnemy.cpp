// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CEnemy.h"

#include "CAnimInstance.h"
#include "Global.h"
#include "../../../Plugins/Developer/RiderLink/Source/RD/thirdparty/clsocket/src/ActiveSocket.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/CMovementComponent.h"
#include "Components/CHealthPointComponent.h"
#include "Weapons/CWeaponStructures.h"


// Sets default values
ACEnemy::ACEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UCHealthPointComponent>(this,&HealthPoint,"HealthPoint");
	CHelpers::CreateActorComponent<UCMovementComponent>(this,&Movement,"Movement");
	CHelpers::CreateActorComponent<UCStateComponent>(this,&State,"State");
	
	//위치,회전잡기
	GetMesh()->SetRelativeLocation(FVector(0,0,-90));
	GetMesh()->SetRelativeRotation(FRotator(0,-90,0));
	//메쉬 붙이기
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh,"SkeletalMesh'/Game/Characters/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);
	//캡슐 붙이기

	//애니메이션
	TSubclassOf<UCAnimInstance> animInstance;
	CHelpers::GetClass<UCAnimInstance>(&animInstance,"AnimBlueprint'/Game/BP/ABP_Character.ABP_Character_C'");

	//이동방향으로 회전에서 회전 속도
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	//맞는 액션 몽타주
	CHelpers::GetAsset<UAnimMontage>(&DamagedMontage,"AnimMontage'/Game/Characters/Montages/HitReaction_Montage.HitReaction_Montage'");
	//죽는 액션 몽타주
	CHelpers::GetAsset<UAnimMontage>(&DeadMontage,"AnimMontage'/Game/Characters/Montages/Enemy_DeadFall_Montage.Enemy_DeadFall_Montage'");
}

// Called when the game starts or when spawned
void ACEnemy::BeginPlay()
{
	Super::BeginPlay();

	Movement->OnWalk();
	
	//메쉬에 있는 머테리얼 가져와서 하나씩 for문 돌리기 => 다이나믹 머테리얼 만들기 위해서
	for (int i = 0; i < GetMesh()->GetMaterials().Num(); i++)
	{
		UMaterialInterface* material = GetMesh()->GetMaterials()[i]; // Tarray

		GetMesh()->SetMaterial(i,UMaterialInstanceDynamic::Create(material,this));
	}

	Change_Color(OriginColor);

	State->OnStateTypeChanged.AddDynamic(this,&ACEnemy::OnStateTypeChanged);
}

void ACEnemy::Change_Color(const FLinearColor& InColor)
{
	//UMaterialInterface => Material의 최상위
	for (UMaterialInterface* material : GetMesh()->GetMaterials())
	{
		UMaterialInstanceDynamic* instance = Cast<UMaterialInstanceDynamic>(material);
		
		//instance가 널이 아니라면
		if (!!instance)
		{
			instance->SetVectorParameterValue("Color", InColor);
		}
	}
}

void ACEnemy::RestoreColor()
{
	Change_Color(OriginColor);
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Damaged:
			Damaged(); break;
		case EStateType::Dead :
			Dead(); break;
	}
}

void ACEnemy::Damaged()
{
	
	//Apply Damage
	{
		//실제 데미지 처리
		HealthPoint->Damage(DamageData.Power);
		//쓰고나서 초기화
		DamageData.Power = 0.0f;
	}
	
	
	//Change Color
	{
		Change_Color(FLinearColor::Red);

		//정의 확인 후 인자값 개수 확인
		FTimerDelegate TimerDelegate;
		//어디에 있는 함수인지
		TimerDelegate.BindUFunction(this,"RestoreColor");

		//0.2f => 얼마뒤에 콜할건지
		GetWorld()->GetTimerManager().SetTimer(ChangeColor_TimerHandle,TimerDelegate,0.2f,false);
	}
	

	if (!!DamageData.Event && !!DamageData.Event->HitData)
	{
		FHitData* HitData = DamageData.Event->HitData;

		UAnimMontage* Montage = HitData->Montage;
		float PlayRate = HitData -> PlayRate;

		if (Montage == nullptr)
		{
			Montage = DamagedMontage;
			PlayRate = 1.5f;
		}
		
		PlayAnimMontage(Montage,PlayRate);
		
		HitData->PlayHitStop(GetWorld());
		HitData->PlaySoundWave(this);
		HitData->PlayEffect(GetWorld(),GetActorLocation(),GetActorRotation());

		//Launch
		if (HealthPoint->IsDead() == false)
		{
			FVector Start = GetActorLocation();
			FVector Target = DamageData.Attacker->GetActorLocation();
			FVector Direction = Target - Start;
			Direction.Normalize();
		
			LaunchCharacter(-Direction * HitData->Launch, false,false);
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(Start,Target));
		}
	}

	if (HealthPoint->IsDead())
	{
		State->SetDeadMode();
		return;
	}

	//다 쓴 데이터 비우기
	DamageData.Attacker = nullptr;
	DamageData.Causer = nullptr;
	DamageData.Event = nullptr;	
}


void ACEnemy::Dead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	PlayAnimMontage(DeadMontage);

	//Destroy(this);
}

void ACEnemy::End_Dead()
{
	Destroy();
}

float ACEnemy::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator,
                          AActor* DamageCauser)
{
	Damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	DamageData.Power = Damage;
	DamageData.Attacker = Cast<ACharacter>(EventInstigator -> GetPawn()); //다운 캐스팅
	DamageData.Causer = DamageCauser;
	DamageData.Event = (FActionDamageEvent*)&DamageEvent;

	State->SetDamagedMode();
	
	//포인터로 캐스팅해야 널체크가 가능하다 DamageEvent주소를 FActionDamageEvent로 캐스팅 해보겠다는 뜻
	FActionDamageEvent* e = (FActionDamageEvent*) &DamageEvent;

	// if (!!e)
	// {
	// 	if (!!e->HitData)
	// 	{
	// 		CLog::Print(e->HitData->StopTime);	
	// 	}
	// }
	
	return Damage;
	//부모에서 데미지 처리할게 있으면 , 서버처리 등 최종 데미지가 요기로 옴
}

void ACEnemy::End_Damaged()
{
	State->SetIdleMode();
}




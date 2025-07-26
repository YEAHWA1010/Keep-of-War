// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AddOns/CArrow.h"
#include "Global.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

// Sets default values
ACArrow::ACArrow()
{
	CHelpers::CreateComponent<UCapsuleComponent>(this,&Capsule,"Capsule");
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this,&Projectile,"Projectile");

	Projectile->ProjectileGravityScale = 0.0f;

	Capsule->BodyInstance.bNotifyRigidBodyCollision = true;
	Capsule->SetCollisionProfileName("BlockAll");
}

void ACArrow::Shoot(const FVector& InForward)
{
	Projectile->Velocity = InForward * Projectile->InitialSpeed; //초기속도
	Projectile->SetActive(true);

	FTimerHandle handle;
	FTimerDelegate timerDelegate = FTimerDelegate::CreateLambda([&]() //충돌 키는거 지연
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	});
	
	GetWorld()->GetTimerManager().SetTimer(handle,timerDelegate,0.1f,false);
}

// Called when the game starts or when spawned
void ACArrow::BeginPlay()
{
	Super::BeginPlay();

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Capsule->OnComponentHit.AddDynamic(this,&ACArrow::OnComponentHit);
	Projectile->SetActive(false);
}

void ACArrow::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (OnEndPlay.IsBound())
	{
		OnEndPlay.Broadcast(this);
	}
}

void ACArrow::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                             FVector NormalImpulse, const FHitResult& Hit)
{
	//시간을 주면 이 시간 이후로 사라짐
	SetLifeSpan(LifeSpanAfterCollision);

	for (AActor* actor : Ignores)
	{
		CheckTrue(actor == OtherActor);
	}

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (!!Character && OnHit.IsBound())
	{
		OnHit.Broadcast(this,Character);
	}
}

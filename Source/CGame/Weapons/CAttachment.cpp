// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CAttachment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"

// Sets default values
ACAttachment::ACAttachment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<USceneComponent>(this,&Root,"Root");
}

// Called when the game starts or when spawned
void ACAttachment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	TArray<USceneComponent*> children;
	//자식의 자식까지 찾겠다
	Root->GetChildrenComponents(true,children);

	for (USceneComponent* child : children)
	{
		UShapeComponent* shape = Cast<UShapeComponent>(child);

		if (!!shape)
		{
			shape->OnComponentBeginOverlap.AddDynamic(this,&ACAttachment::OnComponentBeginOverlap);
			shape->OnComponentBeginOverlap.AddDynamic(this,&ACAttachment::OnComponentBeginOverlap);
			
			Collisions.Add(shape);
		}
	}

	//게임 시작 후 충돌체 꺼주기
	OffCollisions();
	//노티파이를 통해 충돌 구간에서 켜주기
	
	Super::BeginPlay(); // 블루프린트 begin을 먼저 콜해서 세팅을 위에 해줘야함

	
}

void ACAttachment::OnCollisions()
{
	if (OnFAttachmentBeginCollision.IsBound())
	{
		OnFAttachmentBeginCollision.Broadcast();
	}
	for (UShapeComponent* shape : Collisions)
	{
		shape->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void ACAttachment::OffCollisions()
{
	//콜리전 중복 방지
	if (OnFAttachmentEndCollision.IsBound())
	{
		OnFAttachmentEndCollision.Broadcast();
	}
	for (UShapeComponent* shape : Collisions)
	{
		shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}


void ACAttachment::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//ownercharacter랑 타격한 otheractor랑 같으면 안됨 => 칼뽑다가닿음
	CheckTrue(OwnerCharacter == OtherActor);

	//데미지 처리
	if (OnAttachmentBeginOverlap.IsBound())
	{
		OnAttachmentBeginOverlap.Broadcast(OwnerCharacter,this,Cast<ACharacter>(OtherActor));
	}
}

void ACAttachment::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)

{
	//ownercharacter랑 타격한 otheractor랑 같으면 안됨 => 칼뽑다가닿음
	CheckTrue(OwnerCharacter == OtherActor);
	
	if (OnAttachmentEndOverlap.IsBound())
	{
		OnAttachmentEndOverlap.Broadcast(OwnerCharacter,Cast<ACharacter>(OtherActor));
	}
}

void ACAttachment::AttachTo(FName InSocketname)
{
	AttachToComponent(OwnerCharacter->GetMesh(),FAttachmentTransformRules(EAttachmentRule::KeepRelative,true),InSocketname);
}

void ACAttachment::AttachToCollision(FName InCollisionName)
{
	//모아놓은 충돌체들
	for (UShapeComponent* Collision : Collisions)
	{
		//ToString() 문자열 비교를 위해
		if (Collision->GetName() == InCollisionName.ToString())
		{
			//Collision 충돌체가 가야함 BP_Fist가 가는게 아니고 
			Collision -> AttachToComponent(OwnerCharacter->GetMesh(),FAttachmentTransformRules(EAttachmentRule::KeepRelative,true),InCollisionName);
			return;
		}
	}
}

// Called every frame
void ACAttachment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


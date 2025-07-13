// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CSword.h"

#include "Global.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"


// Sets default values
ACSword::ACSword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this,&Root,"Root");
	CHelpers::CreateComponent<USkeletalMeshComponent>(this,&Mesh,"Mesh",Root);
	CHelpers::CreateComponent<UCapsuleComponent>(this,&Capsule,"Capsule",Mesh);

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh,"SkeletalMesh'/Game/Characters/Weapons/Sword/SK_Sword.SK_Sword'");
	Mesh->SetSkeletalMesh(mesh);

	Capsule->SetRelativeLocation(FVector(-12, -130, 0));
	Capsule->SetRelativeRotation(FRotator(0, 0, 90));
	Capsule->SetCapsuleHalfHeight(220);
	Capsule->SetCapsuleRadius(11);
}

// Called when the game starts or when spawned
void ACSword::BeginPlay()
{
	Super::BeginPlay();

	Capsule->OnComponentBeginOverlap.AddDynamic(this,&ACSword::OnBeginOverlap);
}

// Called every frame
void ACSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACSword::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FDamageEvent e;
	//적에서 받을 수 있는지 확인
	OtherActor->TakeDamage(20,e,GetOwner()->GetInstigatorController(),this);
}

void ACSword::OnCollision()
{
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACSword::OffCollision()
{
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}



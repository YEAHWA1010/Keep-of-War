// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CGhostTrail.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
// Sets default values
ACGhostTrail::ACGhostTrail()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<UPoseableMeshComponent>(this,&Mesh,"Mesh");
}

// Called when the game starts or when spawned
void ACGhostTrail::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	
	UMaterialInstanceConstant* material;
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&material,"MaterialInstanceConstant'/Game/Characters/Materials/M_GhostTrail_Inst.M_GhostTrail_Inst'");

	Material = UMaterialInstanceDynamic::Create(material,this);
	Material->SetVectorParameterValue("Color",Color);
	Material->SetScalarParameterValue("Exponent",Exponent);

	Mesh->SetVisibility(false);
	
	Mesh->SetSkeletalMesh(OwnerCharacter->GetMesh()->SkeletalMesh); //캐릭터의 메쉬를 대상으로 캡쳐할 예정
	//CopyPoseFromSkeletalComponent 캡쳐는 무조건 한번은 이루어 져야한다.
	Mesh->CopyPoseFromSkeletalComponent(OwnerCharacter->GetMesh());
	Mesh->SetRelativeScale3D(Scale);

	//만든거로 할당
	for (int32 i = 0; i < OwnerCharacter->GetMesh()->SkeletalMesh->GetMaterials().Num(); i++)
	{
		Mesh->SetMaterial(i,Material);
	}

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([=, this]()  //명시적으로 this 캡처
	{
		if (Mesh->IsVisible() == false)
		{
			Mesh->ToggleVisibility();

			SetActorLocation(OwnerCharacter->GetActorLocation()+ FVector(0,0,-90));
			SetActorRotation(OwnerCharacter->GetActorRotation() + FRotator(0,-90,0));
		}
		
		Mesh->CopyPoseFromSkeletalComponent(OwnerCharacter->GetMesh());
	});

	//delay 5초 뒤에 실행하고 interval 1초마다 반복 실행
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,TimerDelegate,Interval,true,StartDelay);
}

void ACGhostTrail::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

// Called every frame
void ACGhostTrail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.

#include "Collision/C01_ActorOverlap.h"
#include "CGame.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/BoxComponent.h"

AC01_ActorOverlap::AC01_ActorOverlap()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Root);

	Box->bHiddenInGame = false;
	Box->SetRelativeScale3D(FVector(3));
}

void AC01_ActorOverlap::BeginPlay()
{
	Super::BeginPlay();
	
	Box->OnComponentBeginOverlap.AddDynamic(this, &AC01_ActorOverlap::OnComponentBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &AC01_ActorOverlap::OnComponentEndOverlap);
}

void AC01_ActorOverlap::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	OtherActor = Cast<ACPlayer>(OtherActor);
	if (OtherActor)
	{
		OnTriggered.Broadcast(); // 이벤트 발생
	}
}

void AC01_ActorOverlap::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// FString str;
	//
	// str = "-----End-----";
	// CLog::Log(str);
	//
	// str = FString::Printf(L"Overlap Component : %s", *OverlappedComponent->GetName());
	// CLog::Log(str);
	//
	// str = FString::Printf(L"Other Actor : %s", *OtherActor->GetName());
	// CLog::Log(str);
	//
	// str = FString::Printf(L"Other Component : %s", *OtherComp->GetName());
	// CLog::Log(str);
}


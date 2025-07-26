// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Attachments/ParkourActor.h"
#include "Global.h"
// Sets default values
AParkourActor::AParkourActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<UStaticMeshComponent>(this,&StaticMesh,"StaticMesh",RootComponent);
}


void AParkourActor::BeginPlay()
{
    Super::BeginPlay();

    // FVector Origin, BoxExtent;
    // StaticMesh->GetLocalBounds(Origin, BoxExtent);
    //
    // FVector CurrentSize = BoxExtent * 2.0f;
    // FVector TargetSize = FVector(199.0f, 336.0f, 577.0f);
    //
    // FVector Scale = TargetSize / CurrentSize;
    // StaticMesh->SetRelativeScale3D(Scale);
}

void AParkourActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FVector Origin, BoxExtent;
	StaticMesh->GetLocalBounds(Origin, BoxExtent);

	FVector CurrentSize = BoxExtent * 2.0f;
	FVector TargetSize = FVector(199.0f, 336.0f, 577.0f);

	FVector Scale = TargetSize / CurrentSize;
	StaticMesh->SetRelativeScale3D(Scale);
}

// Called every frame
void AParkourActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


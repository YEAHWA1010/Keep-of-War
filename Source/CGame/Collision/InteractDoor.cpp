// Fill out your copyright notice in the Description page of Project Settings.


#include "Collision/InteractDoor.h"

#include "C01_ActorOverlap.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TimelineComponent.h"
#include "Utilities/CHelpers.h"

// Sets default values
AInteractDoor::AInteractDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AInteractDoor::BeginPlay()
{
	Super::BeginPlay();

	if (CurveFloat)
	{
		FOnTimelineFloat TimeLineProgress;
		TimeLineProgress.BindDynamic(this,&AInteractDoor::OpenDoor);
		TimeLine.AddInterpFloat(CurveFloat,TimeLineProgress);

		AC01_ActorOverlap* Trigger = CHelpers::FindActor<AC01_ActorOverlap>(GetWorld());
		CheckNull(Trigger);

		Trigger->OnTriggered.AddDynamic(this, &AInteractDoor::HandleTriggered);
	}
}

// Called every frame
void AInteractDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimeLine.TickTimeline(DeltaTime);
}

void AInteractDoor::HandleTriggered()
{
	TimeLine.PlayFromStart();
}

void AInteractDoor::OpenDoor(float Value)
{
	FVector DoorLocation = DoorMesh->GetRelativeLocation();
	DoorLocation.Z += 180;
	DoorMesh->SetRelativeLocation(FVector(DoorLocation.X,DoorLocation.Y,DoorLocation.Z * Value)); 
}


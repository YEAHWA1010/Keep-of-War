// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CPatrolPath.h"
#include "Global.h"
#include "Components/SplineComponent.h"
#include "Components/TextRenderComponent.h"

// Sets default values
ACPatrolPath::ACPatrolPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<USplineComponent>(this, &Spline, "Spline", Root);
	CHelpers::CreateComponent<UTextRenderComponent>(this, &Text, "Text", Root);

	Spline->SetRelativeLocation(FVector(0, 0, 30)); //지면보다 위로

	Text->SetRelativeLocation(FVector(0, 0, 120));
	Text->SetRelativeRotation(FRotator(0, 180, 0));
	Text->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	Text->TextRenderColor = FColor::Red;
	Text->bHiddenInGame = true;

}

void ACPatrolPath::OnConstruction(const FTransform& Transform)
{
	bRunConstructionScriptOnDrag = false;

	//GetActorLabel()는 아웃라이너에 있는 이름이 나옴 , 에디터 모드에서만 쓸 수 있음 , 빌드할때 에러 주의
	//Text->Text = fGetActorLabel()

	Spline->SetClosedLoop(bLoop);
}

// Called when the game starts or when spawned
void ACPatrolPath::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector ACPatrolPath::GetMoveTo()
{
	return Spline->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);
}

void ACPatrolPath::UpdateIndex()
{
	int32 Count = Spline->GetNumberOfSplinePoints();

	if (bReverse)
	{
		if (Index > 0)
		{
			Index--;

			return;
		}

		if (Spline->IsClosedLoop())
		{
			Index = Count - 1;

			return;
		}

		Index = 1;
		bReverse = false;

		return;
	}

	if (Index < Count - 1)
	{
		Index++;

		return;
	}

	if (Spline->IsClosedLoop())
	{
		Index = 0;

		return;
	}

	Index = Count - 2;
	bReverse = true;
}

// Called every frame
void ACPatrolPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


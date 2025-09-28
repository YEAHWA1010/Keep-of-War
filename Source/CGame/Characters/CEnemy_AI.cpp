// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/CEnemy_AI.h"
#include "CGame.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Components/CHealthPointComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/WidgetComponent.h"
#include "Weapons/CAttachment.h"
#include "Widgets/CUserWidget_Enemy.h"
#include "Widgets/CEnemyLockOnWidget.h"

#if WITH_EDITOR
#endif

void ACEnemy_AI::BeginPlay()
{
	Super::BeginPlay();
	
	HealthPoint->OnHealthPointChanged.AddDynamic(this, &ACEnemy_AI::OnHealthPointChanged);
	
	LabelWidget->InitWidget();
	CursorWidget->InitWidget();

	UCUserWidget_Enemy* label = Cast<UCUserWidget_Enemy>(LabelWidget->GetUserWidgetObject());
	label->UpdateHealth(HealthPoint->GetHealth(), HealthPoint->GetMaxHealth());
	label->UpdateName(GetName());
	label->UpdateControllerName(GetController()->GetName());

	UCEnemyLockOnWidget* Cursor = Cast<UCEnemyLockOnWidget>(CursorWidget->GetUserWidgetObject());
	if (!!Cursor)
	{
		//CLog::Print("nocursor");
		Cursor->SetTargetCursor(false);	
	}
	
}

void ACEnemy_AI::Damaged()
{
	Super::Damaged();
	CheckTrue(State->IsDeadMode());

	Behavior->SetHittedMode();
}

void ACEnemy_AI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Tick_LabelRenderScale();
}

void ACEnemy_AI::End_Damaged()
{
	Super::End_Damaged();
	Behavior->SetWaitMode();
}

void ACEnemy_AI::End_Dead()
{
	if (Weapon->GetAttachment())
	{
		Weapon->GetAttachment()->Destroy();
	}
	
	Super::End_Dead();
}

UUserWidget* ACEnemy_AI::GetCursorWidget()
{
	return CursorWidget->GetUserWidgetObject();
}

ACEnemy_AI::ACEnemy_AI()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<UWidgetComponent>(this, &LabelWidget, "Label", GetMesh());
	CHelpers::CreateComponent<UWidgetComponent>(this, &CursorWidget, "Cursor", GetMesh());
	
	CHelpers::CreateActorComponent<UCWeaponComponent>(this,&Weapon,"Weapon");
	CHelpers::CreateActorComponent<UCAIBehaviorComponent>(this, &Behavior, "Behavior");

	TSubclassOf<UCUserWidget_Enemy> labelClass;
	CHelpers::GetClass(&labelClass, "WidgetBlueprint'/Game/Widgets/WB_Enemy.WB_Enemy_C'");

	TSubclassOf<UCEnemyLockOnWidget> cursorClass;
	CHelpers::GetClass(&cursorClass, "WidgetBlueprint'/Game/Widgets/WB_EnemyCursor.WB_EnemyCursor_C'");

	LabelWidget->SetWidgetClass(labelClass);
	
	LabelWidget->SetRelativeLocation(FVector(0, 0, 220));
	LabelWidget->SetDrawSize(FVector2D(120, 0));
	LabelWidget->SetWidgetSpace(EWidgetSpace::Screen);

	CursorWidget->SetWidgetClass(cursorClass);
	
	CursorWidget->SetRelativeLocation(FVector(0, 0, 90));
	CursorWidget->SetDrawSize(FVector2D(120, 0));
	CursorWidget->SetWidgetSpace(EWidgetSpace::Screen);

	
	CHelpers::GetAsset<UBehaviorTree>(&BehaviorTree,"BehaviorTree'/Game/BP/Enemies/Melee/BT_Melee.BT_Melee'");
}
void ACEnemy_AI::Tick_LabelRenderScale()
{
	UCUserWidget_Enemy* label = Cast<UCUserWidget_Enemy>(LabelWidget->GetUserWidgetObject());
	CheckNull(label);

	
	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	FVector cameraLocation = cameraManager->GetCameraLocation();
	FVector targetLocation = GetController()->GetTargetLocation();

	float distance = FVector::Distance(cameraLocation, targetLocation);
	float sizeRate = 1.0f - (distance / MaxLabelDistance);

	if (distance > MaxLabelDistance)
	{
		label->SetVisibility(ESlateVisibility::Collapsed);

		return;
	}

	label->SetVisibility(ESlateVisibility::Visible);
	label->SetRenderScale(FVector2D(sizeRate, sizeRate));
}

void ACEnemy_AI::OnHealthPointChanged(float InHealth, float InMaxHealth)
{
	UCUserWidget_Enemy* label = Cast<UCUserWidget_Enemy>(LabelWidget->GetUserWidgetObject());
	label->UpdateHealth(HealthPoint->GetHealth(), HealthPoint->GetMaxHealth());
}

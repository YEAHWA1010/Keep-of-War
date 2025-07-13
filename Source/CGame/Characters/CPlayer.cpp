// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Global.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CAnimInstance.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Weapons/CWeaponStructures.h"
#include "Chaos/EPA.h"
#include "Components/CStateComponent.h"
#include "Components/CTargetComponent.h"
#include "DSP/AudioDebuggingUtilities.h"
#include "Widgets/CUserWidget_Player.h"
#include "Components/CHealthPointComponent.h"

// Sets default values
ACPlayer::ACPlayer()
{
	//캡슐에 붙임
	CHelpers::CreateComponent<USpringArmComponent>(this,&SpringArm,"SpringArm",GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this,&Camera,"Camera",SpringArm);
	
	CHelpers::CreateActorComponent<UCHealthPointComponent>(this,&HealthPoint,"HealthPoint");
	CHelpers::CreateActorComponent<UCWeaponComponent>(this,&Weapon,"Weapon");
	CHelpers::CreateActorComponent<UCMovementComponent>(this,&Movement,"Movement");
	CHelpers::CreateActorComponent<UCTargetComponent>(this,&Target,"Target");
	CHelpers::CreateActorComponent<UCStateComponent>(this,&State,"State");
	CHelpers::CreateActorComponent<UCParkourComponent>(this,&Parkour,"Parkour");
	
	//Movement = CreateDefaultSubobject<UCMovementComponent>("Movement");
	
	//위치,회전잡기
	GetMesh()->SetRelativeLocation(FVector(0,0,-90));
	GetMesh()->SetRelativeRotation(FRotator(0,-90,0));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh,"SkeletalMesh'/Game/Characters/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UCAnimInstance> animInstance;
	CHelpers::GetClass<UCAnimInstance>(&animInstance,"AnimBlueprint'/Game/BP/ABP_Character.ABP_Character_C'");
	
	GetMesh()->SetAnimClass(animInstance);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true; //이동 방향으로 회전
	GetCharacterMovement() -> MaxWalkSpeed = 400;
	
	
	SpringArm->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(0,90,0));
	SpringArm->TargetArmLength = 200; //카메라 거리 조정
	SpringArm->bDoCollisionTest = false; //?
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	//이동방향으로 회전에서 회전 속도
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	//this 액터 안에 "Arrows"라는 이름의 USceneComponent를 만들고,그걸 ArrowGroup에 저장
	CHelpers::CreateComponent<USceneComponent>(this,&ArrowGroup,"Arrows",GetCapsuleComponent());
	
	for (int32 i = 0; i < (int32)EParkourArrowType::Max; i++)
	{
		//GetNameStringByIndex()=>번호를 넣으면 번호에 맞는 문자열을 리턴
		FString name = StaticEnum<EParkourArrowType>()->GetNameStringByIndex(i);
		CHelpers::CreateComponent<UArrowComponent>(this,&Arrows[i],FName(name),ArrowGroup);

		switch (EParkourArrowType(i))
		{
			case EParkourArrowType::Center:
				Arrows[i]->ArrowColor = FColor::Red;
			break;

		case EParkourArrowType::Head:
				Arrows[i]->ArrowColor = FColor::Green;
				Arrows[i]->SetRelativeLocation(FVector(0,0,100));
			break;

		case EParkourArrowType::Foot:
				Arrows[i]->ArrowColor = FColor::Blue;
				Arrows[i]->SetRelativeLocation(FVector(0,0,-80));
			break;

		case EParkourArrowType::Left:
				Arrows[i]->ArrowColor = FColor::Magenta;
				Arrows[i]->SetRelativeLocation(FVector(0,-30,0));
			break;

		case EParkourArrowType::Right:
			Arrows[i]->ArrowColor = FColor::Magenta;
			Arrows[i]->SetRelativeLocation(FVector(0,30,0));
			break;

		case EParkourArrowType::Land:
			Arrows[i]->ArrowColor = FColor::Yellow;
			Arrows[i]->SetRelativeLocation(FVector(200,0,100));
			Arrows[i]->SetRelativeRotation(FRotator(-90,0,0));
			break;
		}
	}
	
	CHelpers::GetAsset<UAnimMontage>(&BackStepMontage,"/Script/Engine.AnimMontage'/Game/Characters/Montages/BackStep_Montage.BackStep_Montage'");
	CHelpers::GetClass<UCUserWidget_Player>(&UIClass, "WidgetBlueprint'/Game/Widgets/WB_Player.WB_Player_C'");

	//맞는 액션 몽타주
	CHelpers::GetAsset<UAnimMontage>(&DamagedMontage,"AnimMontage'/Game/Characters/Montages/HitReaction_Montage.HitReaction_Montage'");
	
}

float ACPlayer::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{

		Damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

		DamageData.Power = Damage;
		DamageData.Attacker = Cast<ACharacter>(EventInstigator -> GetPawn()); //다운 캐스팅
		DamageData.Causer = DamageCauser;
		DamageData.Event = (FActionDamageEvent*)&DamageEvent;

		State->SetDamagedMode();
	
		FActionDamageEvent* e = (FActionDamageEvent*) &DamageEvent;

		return Damage;
}

// Called when the game starts or when spawned
void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	Movement->OnRun();
	Movement->DisableControlRotation();

	State->OnStateTypeChanged.AddDynamic(this,&ACPlayer::OnStateTypeChanged);
	Weapon->OnWeaponTypeChanged.AddDynamic(this, &ACPlayer::OnWeaponTypeChanged);

	if (!!UIClass)
	{
		UserInterface = CreateWidget<UCUserWidget_Player>(GetController<APlayerController>(), UIClass);
		UserInterface->AddToViewport();
		UserInterface->UpdateWeaponType(EWeaponType::Max);
		UserInterface->UpdateCrossHairVisibility(false);
	}
}

// Called to bind functionality to input
void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// PlayerInputComponent -> BindAxis("MoveForword",this,&ACPlayer::OnMoveForward);
	// PlayerInputComponent -> BindAxis("MoveRight",this,&ACPlayer::OnMoveRight);
	
	//연결
	PlayerInputComponent->BindAxis("MoveForward",Movement,&UCMovementComponent::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight",Movement,&UCMovementComponent::OnMoveRight);
	PlayerInputComponent->BindAxis("VerticalLook",Movement,&UCMovementComponent::OnVerticalLook);
	PlayerInputComponent->BindAxis("HorizontalLook",Movement,&UCMovementComponent::OnHorizontalLook);
	
	PlayerInputComponent->BindAction("Sprint",EInputEvent::IE_Pressed,Movement,&UCMovementComponent::OnSprint);
	PlayerInputComponent->BindAction("Sprint",EInputEvent::IE_Released,Movement,&UCMovementComponent::OnRun);
	PlayerInputComponent->BindAction("Hammer",EInputEvent::IE_Pressed,Weapon,&UCWeaponComponent::SetHammerMode);
	PlayerInputComponent->BindAction("Sword",EInputEvent::IE_Pressed,Weapon,&UCWeaponComponent::SetSwordMode);
	PlayerInputComponent->BindAction("Fist",EInputEvent::IE_Pressed,Weapon,&UCWeaponComponent::SetFistMode);
	PlayerInputComponent->BindAction("Warp",EInputEvent::IE_Pressed,Weapon,&UCWeaponComponent::SetWarpMode);
	PlayerInputComponent->BindAction("Bow",EInputEvent::IE_Pressed,Weapon,&UCWeaponComponent::SetBowMode);
	
	PlayerInputComponent->BindAction("Action",EInputEvent::IE_Pressed,Weapon,&UCWeaponComponent::DoAction);
	PlayerInputComponent->BindAction("Evade",EInputEvent::IE_Pressed,this,&ACPlayer::OnEvade);
	PlayerInputComponent->BindAction("Target",EInputEvent::IE_Pressed,Target,&UCTargetComponent::Toggle);
	PlayerInputComponent->BindAction("Target_Left",EInputEvent::IE_Pressed,Target,&UCTargetComponent::MoveLeft);
	PlayerInputComponent->BindAction("Target_Right",EInputEvent::IE_Pressed,Target,&UCTargetComponent::MoveRight);
	
	PlayerInputComponent->BindAction("SubAction",EInputEvent::IE_Pressed,this,&ACPlayer::OnSubAction);
	PlayerInputComponent->BindAction("SubAction",EInputEvent::IE_Released,this,&ACPlayer::OffSubAction);
	
	//dash도 있음
}


void ACPlayer::OnEvade()
{
	CheckFalse(State->IsIdleMode());
	//false는 이동할 수 없음
	CheckFalse(Movement->CanMove());

	//GetAxisValue는 입력하고 있는 방향의 값을 리턴받을 수 있음
	CheckTrue(InputComponent->GetAxisValue("MoveForward") >= 0.0f);

	//공격에 들어가는가 액션 모드에 들어가는가
	//이벤트에 의해서 뛰도록

	State->SetEvadeMode();
	
}

void ACPlayer::OnSubAction()
{
	if (Weapon->IsUnarmedMode())
	{
		CheckFalse(State->IsIdleMode());
		Parkour->DoParkour();

		return;
	}

	Weapon->SubAction_Pressed();
}

void ACPlayer::OffSubAction()
{
	CheckTrue(Weapon->IsUnarmedMode());
	Weapon->SubAction_Released();
}

void ACPlayer::Landed(const FHitResult& Hit)
{
	//Super::Landed(Hit);
	CheckFalse(State->IsIdleMode());
	Parkour->DoParkour(true);
	//CLog::Print("Landed",9999);
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Damaged:
		Damaged(); break;
	// case EStateType::Dead :
	// 	Dead(); break;
	}
}

void ACPlayer::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	UserInterface->UpdateWeaponType(InNewType);
	
	bool bVisible = false;
	
	if (InNewType == EWeaponType::Bow)
		bVisible = true;

	UserInterface->UpdateCrossHairVisibility(bVisible);
}

void ACPlayer::End_BackStep()
{
	IIStateable::End_BackStep();
}

void ACPlayer::Damaged()
{
	//Apply Damage
	{
		//실제 데미지 처리
		HealthPoint->Damage(DamageData.Power);
		//쓰고나서 초기화
		DamageData.Power = 0.0f;
	}
	
	
	// //Change Color
	// {
	// 	Change_Color(FLinearColor::Red);
	//
	// 	//정의 확인 후 인자값 개수 확인
	// 	FTimerDelegate TimerDelegate;
	// 	//어디에 있는 함수인지
	// 	TimerDelegate.BindUFunction(this,"RestoreColor");
	//
	// 	//0.2f => 얼마뒤에 콜할건지
	// 	GetWorld()->GetTimerManager().SetTimer(ChangeColor_TimerHandle,TimerDelegate,0.2f,false);
	// }
	

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
			FVector TargetLocation = DamageData.Attacker->GetActorLocation();
			FVector Direction = TargetLocation - Start;
			Direction.Normalize();
		
			LaunchCharacter(-Direction * HitData->Launch, false,false);
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(Start,TargetLocation));
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




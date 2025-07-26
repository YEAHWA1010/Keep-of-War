// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CWeaponComponent.generated.h"


UENUM(BluePrintType)
enum class EWeaponType : uint8
{
	Fist,Sword,Hammer,Warp,Bow,Max,	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);

UCLASS()
class CGAME_API UCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE EWeaponType GetType() {return Type;}
	FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponType::Max;}

public:
	class ACAttachment* GetAttachment();
	class UCEquipment* GetEquipment();
	class UCDoAction* GetDoAction();
	class UCSubAction* GetSubAction();
	
private:
	//데이터 테이블 기반 무기 종류별로 가져옴
	//직렬화가 되어서 블루프린트에 뜨게함
	UPROPERTY(EditAnywhere,Category = "DataAsset")
	class UCWeaponAsset* DataAssets[(int32)EWeaponType::Max];
	
public:	
	// Sets default values for this component's properties
	UCWeaponComponent();
	
private:
	bool IsIdleMode();
	
public:
	FWeaponTypeChanged OnWeaponTypeChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetUnarmedMode();
	void SetSwordMode();
	void SetHammerMode();
	void SetFistMode();
	void SetWarpMode();
	void SetBowMode();
	
public:
	void SetMode(EWeaponType InType);
	void ChangeType(EWeaponType InType);
	void DoAction();
	void SubAction_Pressed();
	void SubAction_Released();

private:
	EWeaponType Type = EWeaponType::Max;
	
private:
	class ACharacter* OwnerCharacter;

private:
	UPROPERTY()
	class UCWeaponData* Datas[(int32)EWeaponType::Max];
};




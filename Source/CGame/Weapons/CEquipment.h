// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponStructures.h"
#include "CEquipment.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentBeginEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentUnEquip);

UCLASS()
class CGAME_API UCEquipment : public UObject
{
	GENERATED_BODY()

public:
	void BeginPlay(class ACharacter* InOwner, const FEquipmentData& InData);
	
	FORCEINLINE bool GetBeginEquip() { return bBeginEquip; }
	FORCEINLINE const bool* GetEquipped() { return &bEquipped; }
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void Equip();
	void Equip_Implementation();

	//unequip을 정의하고 필요하면 재정의
	UFUNCTION(BlueprintNativeEvent)
	void UnEquip();
	void UnEquip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void Begin_Equip();
	void Begin_Equip_Implementation();
	
	UFUNCTION(BlueprintNativeEvent)
	void End_Equip();
	void End_Equip_Implementation();

public:
	FEquipmentBeginEquip OnEquipmentBeginEquip;
	FEquipmentUnEquip OnEquipmentUnEquip;

private:
	FEquipmentData Data;
	ACharacter* OwnerCharacter;
private:
	class UCMovementComponent* Movement;
	class UCStateComponent* State;

private:
	//장착 동작 중이 아니라 장착이 완료된 중
	bool bEquipped;
	bool bBeginEquip; //beginequip이 콜된 상황인지 확인용
};

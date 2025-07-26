// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Weapons/CWeaponStructures.h"
#include "CWeaponAsset.generated.h"



//블프에서도 연동가능하게 다이나믹 멀티캐스트로 해줌
UCLASS()
class CGAME_API UCWeaponAsset : public UDataAsset
{
	GENERATED_BODY()

private:
	//블루프린트 코드에서 접근할때만 protected 에디터에서 접근할때는 private
	UPROPERTY(EditAnywhere) // 이 무기가 어떤 모양이 정해진 클래스를 가질건지
	TSubclassOf<class ACAttachment> AttachmentClass;

	UPROPERTY(EditAnywhere) 
	TSubclassOf<class UCEquipment> EquipmentClass;

	UPROPERTY(EditAnywhere)
		FEquipmentData EquipmentData;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCDoAction> DoActionClass;
	
	UPROPERTY(EditAnywhere) 
	TSubclassOf<class UCSubAction> SubActionClass;
	
	UPROPERTY(EditAnywhere)
	TArray<FDoActionData> DoActionDatas;

	UPROPERTY(EditAnywhere)
	TArray<FHitData> HitDatas;

public:
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment;}
	FORCEINLINE class UCEquipment* GetEquipment() { return Equipment;}
	FORCEINLINE class UCDoAction* GetDoAction() { return DoAction;}
	FORCEINLINE class UCSubAction* GetSubAction() { return SubAction;}
	
public:
	UCWeaponAsset();
	void BeginPlay(class ACharacter* InOwner,class UCWeaponData** OutWeaponData);

private:
	
	UPROPERTY()
	class ACAttachment* Attachment;
	UPROPERTY()
	class UCEquipment* Equipment;
	UPROPERTY()
	class UCDoAction* DoAction;
	UPROPERTY()
	class UCSubAction* SubAction;
};

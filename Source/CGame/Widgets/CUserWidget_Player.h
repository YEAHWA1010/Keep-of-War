// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CWeaponComponent.h"
#include "CUserWidget_Player.generated.h"

/**
 * 
 */
UCLASS()
class CGAME_API UCUserWidget_Player : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void UpdateWeaponType(EWeaponType InType);

	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void UpdateCrossHairVisibility(bool bInVisible);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class CGAME_API UCUserWidget_Enemy : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void UpdateHealth(float InHealth, float InMaxHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void UpdateName(const FString& InName);

	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void UpdateControllerName(const FString& InName);
	
};

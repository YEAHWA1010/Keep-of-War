// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CGame.h"
#include "Blueprint/UserWidget.h"
#include "CEnemyLockOnWidget.generated.h"

/**
 * 
 */
UCLASS()
class CGAME_API UCEnemyLockOnWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void SetTargetCursor(bool bCursor);

	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	bool GetCursorState();

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockOn", meta = (AllowPrivateAccess = "true"))
	bool bTargetCursor;
};

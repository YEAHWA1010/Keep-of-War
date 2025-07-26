// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CHealthPointComponent.h"

// Sets default values for this component's properties
UCHealthPointComponent::UCHealthPointComponent()
{

	// ...
}


// Called when the game starts
void UCHealthPointComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
}

void UCHealthPointComponent::Damage(float InAmount)
{
	Health -= (InAmount * 1.0f);
	//값 제한
	Health = FMath::Clamp(Health, 0, MaxHealth); //0, MaxHealth 사이이면 그냥 health가 들어옴

	if (OnHealthPointChanged.IsBound())
		OnHealthPointChanged.Broadcast(Health, MaxHealth);
}



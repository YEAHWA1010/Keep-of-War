// Fill out your copyright notice in the Description page of Project Settings.


#include "CGameMode.h"

#include "Utilities/CHelpers.h"

ACGameMode::ACGameMode()
{
	//ConstructorHelpers::FClassFinder<APawn> asset(L"Blueprint'/Game/BP_CPlayer.BP_CPlayer_C'");
	
	//DefaultPawnClass에 넣어줌
	CHelpers::GetClass<APawn>(&DefaultPawnClass,"Blueprint'/Game/BP/BP_CPlayer.BP_CPlayer_C'");

}

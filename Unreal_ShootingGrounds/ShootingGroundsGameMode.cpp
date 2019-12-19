// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ShootingGroundsGameMode.h"
#include "ShootingGroundsHUD.h"
#include "ShootingGroundsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AShootingGroundsGameMode::AShootingGroundsGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AShootingGroundsHUD::StaticClass();
}

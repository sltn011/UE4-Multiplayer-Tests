// Copyright Epic Games, Inc. All Rights Reserved.

#include "RacingGameMode.h"
#include "RacingPawn.h"
#include "RacingHud.h"

ARacingGameMode::ARacingGameMode()
{
	DefaultPawnClass = ARacingPawn::StaticClass();
	HUDClass = ARacingHud::StaticClass();
}

// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/HUD.h"
#include "RacingHud.generated.h"


UCLASS(config = Game)
class ARacingHud : public AHUD
{
	GENERATED_BODY()

public:
	ARacingHud();

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface
};

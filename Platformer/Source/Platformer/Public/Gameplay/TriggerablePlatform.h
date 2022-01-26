// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/MovingPlatform.h"
#include "TriggerablePlatform.generated.h"

UCLASS()
class PLATFORMER_API ATriggerablePlatform : public AMovingPlatform
{
	GENERATED_BODY()
	
public:

	virtual void MovePlatform(
		float DeltaTime
	);

	void ActivatePlatform(
	);

	void DeactivatePlatform(
	);

private:

	int32 NumActivations = 0;

};

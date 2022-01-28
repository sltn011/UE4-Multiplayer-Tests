// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlatformerPlayerController.generated.h"

UCLASS()
class PLATFORMER_API APlatformerPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay(
	) override;

};

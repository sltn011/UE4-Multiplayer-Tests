// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MenuPlayerPawn.generated.h"

class UCameraComponent;

UCLASS()
class PLATFORMER_API AMenuPlayerPawn : public APawn
{
	GENERATED_BODY()

public:

	AMenuPlayerPawn(
	);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	UCameraComponent *CameraComponent;
};

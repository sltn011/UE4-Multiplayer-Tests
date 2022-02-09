// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameModeBase.generated.h"

UCLASS()
class PLATFORMER_API ALobbyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	ALobbyGameModeBase(
	);

	virtual void PostLogin(
		APlayerController *NewPlayer
	) override;

	virtual void Logout(
		AController *Exiting
	) override;

protected:

	int32 NumPlayers = 0;

};

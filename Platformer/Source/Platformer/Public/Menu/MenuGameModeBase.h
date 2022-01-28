// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Menu/Interface/MenuInterface.h"
#include "MenuGameModeBase.generated.h"

UCLASS()
class PLATFORMER_API AMenuGameModeBase : public AGameModeBase, public IMenuInterface
{
	GENERATED_BODY()
	
public:

	AMenuGameModeBase(
	);

	virtual void StartPlay(
	) override;

	virtual void Host(
	) override;

	virtual void Join(
		const FString &ServerIP
	) override;

protected:

	UFUNCTION()
	void InitMenuOnStart(
	);

	APlayerController *GetPlayerController(
	);

};

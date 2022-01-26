// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PlatformerGameInstance.generated.h"

UCLASS()
class PLATFORMER_API UPlatformerGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	virtual void Init(
	) override;

	UFUNCTION(Exec)
	void HostGame(
	);

	UFUNCTION(Exec)
	void JoinGame(
		const FString &HostIP
	);
};

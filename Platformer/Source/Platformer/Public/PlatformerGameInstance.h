// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
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
	void JoinGameByIP(
		const FString &HostIP
	);

protected:

	void RequestCreateSession(
	);

	void OnCreateSessionComplete(
		FName SessionName,
		bool bSuccessful
	);

	void OnDestroySessionComplete(
		FName SessionName,
		bool bSuccessful
	);

	void OnFindSessionsComplete(
		bool bSuccessful
	);

	FName OnlineSessionName = TEXT("Platformer Game Session");

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearchParams;
};

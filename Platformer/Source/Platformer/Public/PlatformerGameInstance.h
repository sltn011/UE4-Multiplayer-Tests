// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "UObject/UnrealNames.h"
#include "PlatformerGameInstance.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnSessionsFound,
	TArray<FOnlineSessionSearchResult> const &
);

UCLASS()
class PLATFORMER_API UPlatformerGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	virtual void Init(
	) override;

	virtual void Shutdown(
	) override;

	UFUNCTION(Exec)
	void HostGame(
		const FString &SessionName
	);

	UFUNCTION(Exec)
	void JoinGameByIP(
		const FString &HostIP
	);

	void RequestJoinSelectedSession(
		int32 SessionIndex
	);

	void RequestFindSessions(
	);

	FOnSessionsFound OnSessionsFoundDelegate;

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

	void OnJoinedTravel(
		FName SessionName,
		EOnJoinSessionCompleteResult::Type JoinResult
	);

	void OnNetworkFailure(
		UWorld *World,
		UNetDriver *NetDriver,
		ENetworkFailure::Type Failure,
		const FString &Info
	);

	FName DefaultOnlineSessionName = NAME_GameSession; // Name used to create and find session
	FName SpecifiedOnlineSessionName; // Name to be set in session settings with Set

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearchParams;

	FTimerHandle ScanForSessionsTimer;

	bool bShuttingDown = false;

	bool bLanGame = true;
};

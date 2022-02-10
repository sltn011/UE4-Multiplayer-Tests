// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerGameInstance.h"

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

#include "Engine/Engine.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlatformerGameInstance, All, All);

void UPlatformerGameInstance::Init(
) {
    Super::Init();

    IOnlineSubsystem *OSS = IOnlineSubsystem::Get();
    if (OSS) {
        UE_LOG(LogPlatformerGameInstance, Display, TEXT("Found OSS %s!"), *OSS->GetSubsystemName().ToString());

        bLanGame = *OSS->GetSubsystemName().ToString() == FString{ TEXT("NULL") };

        SessionInterface = OSS->GetSessionInterface();
        if (SessionInterface) {
            UE_LOG(LogPlatformerGameInstance, Display, TEXT("Session Interface found!"));

            // Session create finished
            SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(
                this,
                &UPlatformerGameInstance::OnCreateSessionComplete
            );

            // Session destroy finished
            SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(
                this,
                &UPlatformerGameInstance::OnDestroySessionComplete
            );

            // Sessions search finished
            SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(
                this,
                &UPlatformerGameInstance::OnFindSessionsComplete
            );

            // Joining found session
            SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(
                this,
                &UPlatformerGameInstance::OnJoinedTravel
            );

            SessionSearchParams = MakeShared<FOnlineSessionSearch>();
            if (SessionSearchParams.IsValid()) {
                SessionSearchParams->bIsLanQuery = bLanGame;
                SessionSearchParams->MaxSearchResults = 100;
                SessionSearchParams->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

                GetTimerManager().SetTimer(
                    ScanForSessionsTimer,
                    this,
                    &UPlatformerGameInstance::RequestFindSessions,
                    10.0f,
                    true,
                    1.0f
                );
                //RequestFindSessions();
            }
            else {
                UE_LOG(LogPlatformerGameInstance, Warning, TEXT("Error creating session search parameters!"));
            }
            
        }
    }
    else {
        UE_LOG(LogPlatformerGameInstance, Error, TEXT("OSS not found!"));
    }
}

void UPlatformerGameInstance::Shutdown(
) {
    bShuttingDown = true;

    FNamedOnlineSession *ExistingSession = SessionInterface->GetNamedSession(DefaultOnlineSessionName);
    if (ExistingSession) {
        SessionInterface->DestroySession(DefaultOnlineSessionName);
    }
}

void UPlatformerGameInstance::HostGame(
    const FString &SessionName
) {
    UE_LOG(LogPlatformerGameInstance, Display, TEXT("Hosting a game!"));

    if (!SessionInterface.IsValid()) {
        UE_LOG(LogPlatformerGameInstance, Error, TEXT("Online session interface is invalid!"));
        return;
    }

    if (!SessionName.IsEmpty()) {
        SpecifiedOnlineSessionName = FName{ SessionName };
    }
    else {
        SpecifiedOnlineSessionName = DefaultOnlineSessionName;
    }

    // Check if session already exists
    FNamedOnlineSession *ExistingSession = SessionInterface->GetNamedSession(DefaultOnlineSessionName);
    if (ExistingSession) {
        SessionInterface->DestroySession(DefaultOnlineSessionName);
    }
    else {
        RequestCreateSession();
    }
}

void UPlatformerGameInstance::JoinGameByIP(
    const FString &HostIP
) {
    UE_LOG(LogPlatformerGameInstance, Display, TEXT("Joining game at %s"), *HostIP);

    UEngine *Engine = GetEngine();
    if (Engine) {
        Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining game at %s"), *HostIP));
    }

    UWorld *World = GetWorld();
    if (!World) {
        UE_LOG(LogPlatformerGameInstance, Error, TEXT("Unexpected error! Can't join!"));
    }

    APlayerController *Player = GetFirstLocalPlayerController();
    if (!Player) {
        UE_LOG(LogPlatformerGameInstance, Error, TEXT("Unexpected error! Can't join!"));
    }

    Player->ClientTravel(HostIP, ETravelType::TRAVEL_Absolute);
}

void UPlatformerGameInstance::RequestJoinSelectedSession(
    int32 SessionIndex
) {
    if (!SessionInterface.IsValid()) {
        UE_LOG(LogPlatformerGameInstance, Error, TEXT("Online session interface is invalid!"));
        return;
    }

    if (!SessionSearchParams.IsValid()) {
        UE_LOG(LogPlatformerGameInstance, Error, TEXT("Session search parameters are invalid!"));
        return;
    }

    if (SessionIndex >= SessionSearchParams->SearchResults.Num()) {
        UE_LOG(LogPlatformerGameInstance, Error, TEXT("Session index is invalid!"));
        return;
    }

    SessionInterface->JoinSession(0, DefaultOnlineSessionName, SessionSearchParams->SearchResults[SessionIndex]);
}

void UPlatformerGameInstance::RequestFindSessions(
) {
    if (SessionSearchParams.IsValid()) {
        SessionInterface->FindSessions(0, SessionSearchParams.ToSharedRef());
    }
    else {
        UE_LOG(LogPlatformerGameInstance, Warning, TEXT("Session search parameters are invalid!"));
        GetTimerManager().ClearTimer(ScanForSessionsTimer);
    }
}

void UPlatformerGameInstance::RequestCreateSession(
) {
    if (!SessionInterface.IsValid()) {
        UE_LOG(LogPlatformerGameInstance, Error, TEXT("Online session interface is invalid!"));
        return;
    }

    FOnlineSessionSettings SessionSettings;
    SessionSettings.bIsLANMatch = bLanGame;
    SessionSettings.NumPublicConnections = 2;
    SessionSettings.bShouldAdvertise = true;
    SessionSettings.bUsesPresence = true;
    // SessionSettings.bUseLobbiesIfAvailable = true; for >= 4.27
    SessionSettings.Set(
        FName{ TEXT("PLATFORMER_SESSION_NAME") },
        SpecifiedOnlineSessionName.ToString(),
        EOnlineDataAdvertisementType::Type::ViaOnlineServiceAndPing
    );

    SessionInterface->CreateSession(0, DefaultOnlineSessionName, SessionSettings);
}

void UPlatformerGameInstance::OnCreateSessionComplete(
    FName SessionName,
    bool bSuccessful
) {
    if (!bSuccessful) {
        UE_LOG(LogPlatformerGameInstance, Error, TEXT("Unexpected error while creating session! Can't host!"));
        return;
    }

    UEngine *Engine = GetEngine();
    if (Engine) {
        Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting a game!"));
    }

    UWorld *World = GetWorld();
    if (!World) {
        UE_LOG(LogPlatformerGameInstance, Error, TEXT("Unexpected error! Can't host!"));
    }

    GetTimerManager().ClearTimer(ScanForSessionsTimer);

    World->ServerTravel("/Game/Platformer/Maps/Lobby?listen");
}

void UPlatformerGameInstance::OnDestroySessionComplete(
    FName SessionName,
    bool bSuccessful
) {
    if (!bSuccessful) {
        UE_LOG(LogPlatformerGameInstance, Error, TEXT("Unexpected error while destroying session! Can't host!"));
        return;
    }

    if (!bShuttingDown) {
        RequestCreateSession();
    }
    else {
        Super::Shutdown();
    }
}

void UPlatformerGameInstance::OnFindSessionsComplete(
    bool bSuccessful
) {
    UE_LOG(
        LogPlatformerGameInstance,
        Display,
        TEXT("FindSessions: %s!"),
        bSuccessful ? TEXT("Success") : TEXT("Fail")
    );

    if (!SessionSearchParams.IsValid()) {
        UE_LOG(LogPlatformerGameInstance, Error, TEXT("Session search parameters are invalid!"));
        return;
    }

    UE_LOG(
        LogPlatformerGameInstance,
        Display,
        TEXT("Found %d sessions"),
        SessionSearchParams->SearchResults.Num()
    );

    for (FOnlineSessionSearchResult const &FoundSession : SessionSearchParams->SearchResults) {
        UE_LOG(LogPlatformerGameInstance, Display, TEXT("Found session: %s"), *FoundSession.GetSessionIdStr());
    }

    OnSessionsFoundDelegate.Broadcast(SessionSearchParams->SearchResults);
}

void UPlatformerGameInstance::OnJoinedTravel(
    FName SessionName,
    EOnJoinSessionCompleteResult::Type JoinResult
) {
    if (!SessionInterface.IsValid()) {
        UE_LOG(LogPlatformerGameInstance, Error, TEXT("Online session interface is invalid!"));
        return;
    }

    FString ConnectionInfo;
    bool bResult = SessionInterface->GetResolvedConnectString(
        SessionName,
        ConnectionInfo
    );

    if (!bResult) {
        UE_LOG(LogPlatformerGameInstance, Warning, TEXT("Error joining %s session!"), *SessionName.ToString());
        return;
    }

    UE_LOG(LogPlatformerGameInstance, Display, TEXT("Travelling player to %s"), *ConnectionInfo);

    APlayerController *Player = GetFirstLocalPlayerController();
    if (!Player) {
        UE_LOG(LogPlatformerGameInstance, Error, TEXT("Unexpected error! Can't join!"));
        return;
    }

    GetTimerManager().ClearTimer(ScanForSessionsTimer);

    Player->ClientTravel(ConnectionInfo, ETravelType::TRAVEL_Absolute);
}

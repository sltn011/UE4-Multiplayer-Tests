// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerGameInstance.h"

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

#include "Engine/Engine.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlatformerGameInstance, All, All);

void UPlatformerGameInstance::Init(
) {
    Super::Init();

    IOnlineSubsystem *OSS = IOnlineSubsystem::Get(FName{ TEXT("NULL") });
    if (OSS) {
        UE_LOG(LogPlatformerGameInstance, Display, TEXT("Found OSS %s!"), *OSS->GetSubsystemName().ToString());

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

            SessionSearchParams = MakeShared<FOnlineSessionSearch>();
            if (SessionSearchParams.IsValid()) {
                SessionSearchParams->bIsLanQuery = true;
                SessionInterface->FindSessions(0, SessionSearchParams.ToSharedRef());
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

void UPlatformerGameInstance::HostGame(
) {
    UE_LOG(LogPlatformerGameInstance, Display, TEXT("Hosting a game!"));

    if (!SessionInterface.IsValid()) {
        UE_LOG(LogPlatformerGameInstance, Error, TEXT("Online session interface is invalid!"));
        return;
    }

    // Check if session already exists
    FNamedOnlineSession *ExistingSession = SessionInterface->GetNamedSession(OnlineSessionName);
    if (ExistingSession) {
        SessionInterface->DestroySession(OnlineSessionName);
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

void UPlatformerGameInstance::RequestCreateSession(
) {
    if (!SessionInterface.IsValid()) {
        UE_LOG(LogPlatformerGameInstance, Error, TEXT("Online session interface is invalid!"));
        return;
    }

    FOnlineSessionSettings SessionSettings;
    SessionSettings.bIsLANMatch = true;
    SessionSettings.NumPublicConnections = 2;
    SessionSettings.bShouldAdvertise = true;
    SessionInterface->CreateSession(0, OnlineSessionName, SessionSettings);
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

    World->ServerTravel("/Game/Platformer/Maps/GameLevel?listen");
}

void UPlatformerGameInstance::OnDestroySessionComplete(
    FName SessionName,
    bool bSuccessful
) {
    if (!bSuccessful) {
        UE_LOG(LogPlatformerGameInstance, Error, TEXT("Unexpected error while destroying session! Can't host!"));
        return;
    }

    RequestCreateSession();
}

void UPlatformerGameInstance::OnFindSessionsComplete(
    bool bSuccessful
) {
    UE_LOG(LogPlatformerGameInstance, Display, TEXT("Sessions %s!"), bSuccessful ? TEXT("found") : TEXT("not found"));

    if (!SessionSearchParams.IsValid()) {
        return;
    }

    for (FOnlineSessionSearchResult const &FoundSession : SessionSearchParams->SearchResults) {
        UE_LOG(LogPlatformerGameInstance, Display, TEXT("Found session: %s"), *FoundSession.GetSessionIdStr());
    }
}

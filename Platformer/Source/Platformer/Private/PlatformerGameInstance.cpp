// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerGameInstance.h"

#include "OnlineSubsystem.h"

#include "Engine/Engine.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlatformerGameInstance, All, All);

void UPlatformerGameInstance::Init(
) {
    Super::Init();

    IOnlineSubsystem *OSS = IOnlineSubsystem::Get(FName{TEXT("NULL")});
    if (OSS) {
        UE_LOG(LogPlatformerGameInstance, Display, TEXT("Found OSS %s!"), *OSS->GetSubsystemName().ToString());
        IOnlineSessionPtr SessionInterface = OSS->GetSessionInterface();
        if (SessionInterface) {
            UE_LOG(LogPlatformerGameInstance, Display, TEXT("Session Interface found!"));
        }
    }
    else {
        UE_LOG(LogPlatformerGameInstance, Display, TEXT("OSS not found!"));
    }
}

void UPlatformerGameInstance::HostGame(
) {
    UE_LOG(LogPlatformerGameInstance, Display, TEXT("Hosting a game!"));

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

void UPlatformerGameInstance::JoinGame(
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/MenuGameModeBase.h"

#include "Engine/Engine.h"
#include "Menu/MenuPlayerController.h"
#include "Menu/MenuPlayerPawn.h"
#include "Menu/UI/MenuHud.h"
#include "Menu/UI/MenuScreenWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogMenuGameModeBase, All, All);

AMenuGameModeBase::AMenuGameModeBase(
) {
    PlayerControllerClass = AMenuPlayerController::StaticClass();
    DefaultPawnClass = AMenuPlayerPawn::StaticClass();
    HUDClass = AMenuHUD::StaticClass();
}

void AMenuGameModeBase::StartPlay(
) {
    Super::StartPlay();

    UWorld *World = GetWorld();
    if (!World) {
        UE_LOG(LogMenuGameModeBase, Error, TEXT("Unexpected error!"));
    }

    UE_LOG(LogMenuGameModeBase, Error, TEXT("Menu start play!"));
    World->OnWorldBeginPlay.AddUObject(this, &AMenuGameModeBase::InitMenuOnStart);
}

void AMenuGameModeBase::Host(
) {
    UE_LOG(LogMenuGameModeBase, Display, TEXT("Hosting a game!"));

    UWorld *World = GetWorld();
    if (!World) {
        UE_LOG(LogMenuGameModeBase, Error, TEXT("Unexpected error! Can't host!"));
    }

    World->ServerTravel("/Game/Platformer/Maps/GameLevel?listen");
}

void AMenuGameModeBase::Join(
    const FString &ServerIP
) {
    UE_LOG(LogMenuGameModeBase, Display, TEXT("Joining game at %s"), *ServerIP);

    APlayerController *Player = GetPlayerController();
    if (!Player) {
        UE_LOG(LogMenuGameModeBase, Error, TEXT("Can't join!"));
    }

    Player->ClientTravel(ServerIP, ETravelType::TRAVEL_Absolute);
}

void AMenuGameModeBase::InitMenuOnStart(
) {
    APlayerController *Player = GetPlayerController();
    if (!Player) {
        UE_LOG(LogMenuGameModeBase, Error, TEXT("Error initializing menu!"));
    }

    AMenuHUD *HUD = Player->GetHUD<AMenuHUD>();
    if (!HUD) {
        UE_LOG(LogMenuGameModeBase, Error, TEXT("Unexpected error! Can't access player's HUD!"));
    }

    HUD->GetMenuWidget()->SetMenuImplementation(this);
    UE_LOG(LogMenuGameModeBase, Error, TEXT("Menu pointer set!"));
}

APlayerController *AMenuGameModeBase::GetPlayerController(
) {
    UGameInstance *GameInstance = GetGameInstance<UGameInstance>();
    if (!GameInstance) {
        UE_LOG(LogMenuGameModeBase, Error, TEXT("Unexpected error!"));
    }

    UEngine *Engine = GameInstance->GetEngine();
    if (!Engine) {
        UE_LOG(LogMenuGameModeBase, Error, TEXT("Unexpected error!"));
    }

    return Engine->GetFirstLocalPlayerController(GetWorld());
}

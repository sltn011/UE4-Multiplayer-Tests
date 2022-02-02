// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/UI/PlatformerMenuScreenWidget.h"

#include "Components/EditableText.h"
#include "PlatformerGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlatformerMenuScreen, All, All);

void UPlatformerMenuScreenWidget::OnHostButtonClicked(
) {
    UE_LOG(LogPlatformerMenuScreen, Display, TEXT("Host button clicked!"));

    UPlatformerGameInstance *GameInstance = GetGameInstance<UPlatformerGameInstance>();
    if (!GameInstance) {
        UE_LOG(LogPlatformerMenuScreen, Warning, TEXT("Error accessing game instance!"));
    }

    GameInstance->HostGame();
}

void UPlatformerMenuScreenWidget::OnJoinByIPButtonClicked(
) {
    UE_LOG(LogPlatformerMenuScreen, Display, TEXT("Join button clicked!"));

    UPlatformerGameInstance *GameInstance = GetGameInstance<UPlatformerGameInstance>();
    if (!GameInstance) {
        UE_LOG(LogPlatformerMenuScreen, Warning, TEXT("Error accessing game instance!"));
    }

    if (!ServerIPInput) {
        UE_LOG(LogPlatformerMenuScreen, Warning, TEXT("Error accessing server IP input!"));
    }

    FString ServerIP = ServerIPInput->GetText().ToString();
    GameInstance->JoinGameByIP(ServerIP);
}

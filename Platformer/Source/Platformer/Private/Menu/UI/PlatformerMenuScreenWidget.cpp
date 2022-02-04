// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/UI/PlatformerMenuScreenWidget.h"

#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Components/Spacer.h"
#include "Menu/UI/PlatformerSessionSelectWidget.h"
#include "OnlineSessionSettings.h"
#include "PlatformerGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlatformerMenuScreen, All, All);

bool UPlatformerMenuScreenWidget::Initialize(
) {
    bool bParent = Super::Initialize();

    if (!bParent) {
        return false;
    }

    UPlatformerGameInstance *GameInstance = GetGameInstance<UPlatformerGameInstance>();
    if (!GameInstance) {
        UE_LOG(LogPlatformerMenuScreen, Warning, TEXT("Error accessing game instance!"));
    }
    else {
        GameInstance->OnSessionsFoundDelegate.AddUObject(this, &UPlatformerMenuScreenWidget::OnSessionsFound);
    }

    return true;
}

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

void UPlatformerMenuScreenWidget::OnSessionsFound(
    const TArray<FOnlineSessionSearchResult> &Sessions
) {
    if (!SessionSelectorClass) {
        UE_LOG(LogPlatformerMenuScreen, Warning, TEXT("Session selector class is invalid!"));
        return;
    }

    if (!AvailableSessions) {
        UE_LOG(LogPlatformerMenuScreen, Warning, TEXT("Sessions scrollbox not found!"));
        return;
    }

    AvailableSessions->ClearChildren();
    
    int32 Cnt = 0;
    for (FOnlineSessionSearchResult const &Session : Sessions) {
        if (Session.IsValid()) {
            UPlatformerSessionSelectWidget *SessionSelector =
                CreateWidget<UPlatformerSessionSelectWidget>(GetWorld(), SessionSelectorClass);
            if (!SessionSelector) {
                continue;
            }

            SessionSelector->FillSessionData(Session.Session, Cnt);

            AvailableSessions->AddChild(SessionSelector);
        }
        ++Cnt;
    }
}

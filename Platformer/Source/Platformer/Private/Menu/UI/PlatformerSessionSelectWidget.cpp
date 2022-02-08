// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/UI/PlatformerSessionSelectWidget.h"

#include "Components/Button.h"
#include "OnlineSessionSettings.h"
#include "PlatformerGameInstance.h"

bool UPlatformerSessionSelectWidget::Initialize(
) {
    bool bParent = Super::Initialize();

    if (!bParent) {
        return false;
    }

    if (SessionButton) {
        SessionButton->OnClicked.AddDynamic(this, &UPlatformerSessionSelectWidget::OnSessionButtonClick);
    }

    return true;
}

void UPlatformerSessionSelectWidget::FillSessionData(
    FOnlineSession const &Session,
    int32 SessionIndex
) {
    SessionData.IDString = Session.GetSessionIdStr();
    SessionData.Name = TEXT("TEST");
    SessionData.HostName = Session.OwningUserName;
    SessionData.OpenPlayerSlots = Session.SessionSettings.NumPublicConnections - Session.NumOpenPublicConnections;
    SessionData.MaxPlayers = Session.SessionSettings.NumPublicConnections;

    SessionData.SessionIndex = SessionIndex;
}

bool UPlatformerSessionSelectWidget::GetSessionData(
    FSessionData &Result
) const {
    Result = SessionData;
    return true;
}

void UPlatformerSessionSelectWidget::OnSessionButtonClick(
) {
    UPlatformerGameInstance *GameInstance = GetGameInstance<UPlatformerGameInstance>();
    if (!GameInstance) {
        return;
    }

    GameInstance->RequestJoinSelectedSession(SessionData.SessionIndex);
}

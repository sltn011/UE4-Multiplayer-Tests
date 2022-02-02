// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/UI/PlatformerSessionSelectWidget.h"

#include "Components/Button.h"
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

void UPlatformerSessionSelectWidget::SetSessionIDString(
    FString const &NewSessionIDString
) {
    SessionIDString = NewSessionIDString;
}

bool UPlatformerSessionSelectWidget::GetSessionIDString(
    FString &Result
) const {
    Result = SessionIDString;
    return true;
}

void UPlatformerSessionSelectWidget::OnSessionButtonClick(
) {
    UPlatformerGameInstance *GameInstance = GetGameInstance<UPlatformerGameInstance>();
    if (!GameInstance) {
        return;
    }

    GameInstance->JoinGameBySessionID(SessionIDString);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MenuScreenWidget.h"

#include "Components/Button.h"
#include "Components/EditableText.h"

DEFINE_LOG_CATEGORY_STATIC(LogMenuWidget, All, All);

bool UMenuScreenWidget::Initialize(
) {
    bool ParentVal = Super::Initialize();

    check(HostButton);
    check(JoinButton);
    check(ServerIPInput);

    HostButton->OnClicked.AddDynamic(this, &UMenuScreenWidget::OnHostButtonClicked);
    JoinButton->OnClicked.AddDynamic(this, &UMenuScreenWidget::OnJoinButtonClicked);

    return ParentVal;
}

void UMenuScreenWidget::OnHostButtonClicked(
) {
    UE_LOG(LogMenuWidget, Display, TEXT("Host button clicked!"));
}

void UMenuScreenWidget::OnJoinButtonClicked(
) {
    FText Input = ServerIPInput->GetText();
    FString InputString = Input.ToString();

    UE_LOG(LogMenuWidget, Display, TEXT("Join button clicked, Server IP is %s"), *InputString);
}
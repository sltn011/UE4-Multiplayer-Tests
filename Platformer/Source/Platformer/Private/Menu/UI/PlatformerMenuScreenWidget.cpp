// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/UI/PlatformerMenuScreenWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlatformerMenuScreen, All, All);

void UPlatformerMenuScreenWidget::OnHostButtonClicked(
) {
    UE_LOG(LogPlatformerMenuScreen, Display, TEXT("HOST IN DERIVED"));
}

void UPlatformerMenuScreenWidget::OnJoinButtonClicked(
) {
    UE_LOG(LogPlatformerMenuScreen, Display, TEXT("JOIN IN DERIVED"));
}

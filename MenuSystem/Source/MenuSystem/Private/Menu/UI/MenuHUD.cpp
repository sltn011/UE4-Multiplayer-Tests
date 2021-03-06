// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/UI/MenuHUD.h"

#include "Menu/UI/MenuScreenWidget.h"

void AMenuHUD::BeginPlay(
) {
    Super::BeginPlay();

    check(MenuWidgetClass);
    UMenuScreenWidget *MenuWidget = CreateWidget<UMenuScreenWidget>(GetWorld(), MenuWidgetClass);
    if (MenuWidget) {
        MenuWidget->AddToViewport();
    }

}
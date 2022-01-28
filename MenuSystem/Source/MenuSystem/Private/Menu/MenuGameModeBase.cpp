// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/MenuGameModeBase.h"

#include "Menu/MenuPlayerController.h"
#include "Menu/MenuPlayerPawn.h"
#include "Menu/UI/MenuHud.h"

AMenuGameModeBase::AMenuGameModeBase(
) {
    PlayerControllerClass = AMenuPlayerController::StaticClass();
    DefaultPawnClass = AMenuPlayerPawn::StaticClass();
    HUDClass = AMenuHUD::StaticClass();
}
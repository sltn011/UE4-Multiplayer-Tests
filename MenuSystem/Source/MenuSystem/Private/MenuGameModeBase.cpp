// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuGameModeBase.h"

#include "MenuPlayerController.h"
#include "MenuPlayerPawn.h"
#include "UI/MenuHud.h"

AMenuGameModeBase::AMenuGameModeBase(
) {
    PlayerControllerClass = AMenuPlayerController::StaticClass();
    DefaultPawnClass = AMenuPlayerPawn::StaticClass();
    HUDClass = AMenuHUD::StaticClass();
}
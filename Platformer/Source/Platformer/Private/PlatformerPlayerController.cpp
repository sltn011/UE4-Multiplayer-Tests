// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerPlayerController.h"

void APlatformerPlayerController::BeginPlay(
) {
    Super::BeginPlay();

    SetInputMode(FInputModeGameOnly{});
    bShowMouseCursor = false;
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/TriggerablePlatform.h"

DEFINE_LOG_CATEGORY_STATIC(LogTriggerablePlatform, All, All);

void ATriggerablePlatform::MovePlatform(
    float DeltaTime
) {
    if (NumActivations > 0) {
        Super::MovePlatform(DeltaTime);
    }
}

void ATriggerablePlatform::ActivatePlatform(
) {
    ++NumActivations;
    UE_LOG(
        LogTriggerablePlatform,
        Display,
        TEXT("Activated platform %s %d times"),
        *GetName(),
        NumActivations
    );
}

void ATriggerablePlatform::DeactivatePlatform(
) {
    NumActivations = FMath::Max(NumActivations - 1, 0);
    UE_LOG(
        LogTriggerablePlatform,
        Display,
        TEXT("Platform %s got deactivated and now has %d activations"),
        *GetName(),
        NumActivations
    );
}

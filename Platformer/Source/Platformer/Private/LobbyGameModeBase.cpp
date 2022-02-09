// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameModeBase.h"
#include "UObject/ConstructorHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogLobbyGameModeBase, All, All);

ALobbyGameModeBase::ALobbyGameModeBase(
) {
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
}

void ALobbyGameModeBase::PostLogin(
    APlayerController *NewPlayer
) {
    Super::PostLogin(NewPlayer);

    ++NumPlayers;
    if (NumPlayers == 2) {

        UWorld *World = GetWorld();
        if (!World) {
            UE_LOG(LogLobbyGameModeBase, Error, TEXT("Unexpected error! Can't travel!"));
        }

        bUseSeamlessTravel = true;
        World->ServerTravel("/Game/Platformer/Maps/GameLevel?listen");
    }
}

void ALobbyGameModeBase::Logout(
    AController *Exiting
) {
    Super::Logout(Exiting);

    --NumPlayers;
}

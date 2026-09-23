// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopGameMode.h"
#include "CoopGameState.h"
#include "CoopPlayerState.h"
#include "CoopPlayerController.h"
#include "CoopCharacter.h"
#include "GameFramework/PlayerState.h"

ACoopGameMode::ACoopGameMode()
{
    GameStateClass = ACoopGameState::StaticClass();
    PlayerStateClass = ACoopPlayerState::StaticClass();
    PlayerControllerClass = ACoopPlayerController::StaticClass();
    DefaultPawnClass = ACoopCharacter::StaticClass();
}

void ACoopGameMode::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("CoopGameMode::BeginPlay() called"));
}

void ACoopGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if(NewPlayer)
    {
        UE_LOG(LogTemp, Warning, TEXT("PostLogin: Player connected!"));
    }
}

void ACoopGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);

    if(Exiting)
    {
        UE_LOG(LogTemp, Warning, TEXT("Logout: Player disconnected!"));
    }
}
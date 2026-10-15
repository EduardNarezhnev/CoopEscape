// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopGameMode.h"
#include "CoopGameState.h"
#include "CoopPlayerState.h"
#include "CoopPlayerController.h"
#include "CoopCharacter.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

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
    CurrentState = EGameState::InProgress;
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

void ACoopGameMode::GameOver(bool bWon)
{
    if(CurrentState == EGameState::GameOver) return;

    CurrentState = EGameState::GameOver;

    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->SetPause(true);
    }
    
    for(FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if(ACoopPlayerController* PC = Cast<ACoopPlayerController>(It->Get()))
        {
            PC->Client_ShowGameOver(bWon);
        }
    }
}

void ACoopGameMode::RestartGame()
{
    FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
    GetWorld()->ServerTravel(CurrentLevelName + "?listen");
}
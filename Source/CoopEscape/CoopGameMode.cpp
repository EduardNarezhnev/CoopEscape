#include "CoopGameMode.h"
#include "CoopGameState.h"
#include "CoopPlayerController.h"
#include "CoopCharacter.h"
#include "CoopPlayerState.h"
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
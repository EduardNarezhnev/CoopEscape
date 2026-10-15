// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPlayerController.h"
#include "CoopHUD.h"
#include "CoopMainMenu.h"
#include "CoopGameOverScreen.h"
#include "CoopScreenEffect.h"
#include "CoopPauseMenu.h"
#include "CoopGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "CoopPlayerState.h"
#include "GameFramework/GameStateBase.h"

void ACoopPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    if(!IsLocalController()) return;

    if(DefaultMappingContext)
    {
        if(UEnhancedInputLocalPlayerSubsystem* Subsystem = 
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

    FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(this);

    if(CurrentLevel == "MainMenu")
    {
        ShowMainMenu();
    }
    else
    {
        ShowHUD();
    }
}

void ACoopPlayerController::ShowMainMenu()
{
    if(!MainMenuClass) return;

    MainMenuWidget = CreateWidget<UCoopMainMenu>(this, MainMenuClass);
    if(MainMenuWidget)
    {
        MainMenuWidget->AddToViewport();
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
    }
}

void ACoopPlayerController::ShowHUD()
{
    if(!HUDClass) return;

    HUDWidget = CreateWidget<UCoopHUD>(this, HUDClass);
    if(HUDWidget)
    {
        HUDWidget->AddToViewport();
    }

    SetInputMode(FInputModeGameOnly());
    bShowMouseCursor = false;
}


void ACoopPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(HUDWidget)
    {
        HUDWidget->UpdateHUD();
    }
}

void ACoopPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if(UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
    {
        if(PauseAction)
        {
            EnhancedInput->BindAction(PauseAction, ETriggerEvent::Started, this, &ACoopPlayerController::TogglePause);
        }
    }
}

void ACoopPlayerController::TogglePause()
{
   Server_TogglePause();
}

void ACoopPlayerController::Server_TogglePause_Implementation()
{
     bIsPaused = !bIsPaused;
    UE_LOG(LogTemp, Warning, TEXT("Server_TogglePause: bIsPaused = %s"), bIsPaused ? TEXT("true") : TEXT("false"));

    // Ставим паузу НАПРЯМУЮ на сервере
    SetPause(bIsPaused);
    UE_LOG(LogTemp, Warning, TEXT("SetPause(%s) called on server"), bIsPaused ? TEXT("true") : TEXT("false"));

    // Рассылаем клиентам
    if (AGameStateBase* GS = GetWorld()->GetGameState())
    {
        for (APlayerState* PS : GS->PlayerArray)
        {
            if (ACoopPlayerState* CoopPS = Cast<ACoopPlayerState>(PS))
            {
                if (ACoopPlayerController* PC = Cast<ACoopPlayerController>(CoopPS->GetOwner()))
                {
                    PC->Client_SetPause(bIsPaused);
                }
            }
        }
    }
}

void ACoopPlayerController::Client_SetPause_Implementation(bool bPause)
{
     SetPause(bPause);

    if (bPause)
    {
        if (PauseMenuClass)
        {
            PauseWidget = CreateWidget<UCoopPauseMenu>(this, PauseMenuClass);
            if (PauseWidget)
            {
                PauseWidget->AddToViewport();
            }
        }

        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;

        if (HUDWidget)
        {
            HUDWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }
    else
    {
        if (PauseWidget)
        {
            PauseWidget->RemoveFromParent();
            PauseWidget = nullptr;
        }

        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;

        if (HUDWidget)
        {
            HUDWidget->SetVisibility(ESlateVisibility::Visible);
        }
    }
}

void ACoopPlayerController::Client_ShowGameOver_Implementation(bool bWon)
{
    if(!GameOverClass) return;

    GameOverWidget = CreateWidget<UCoopGameOverScreen>(this, GameOverClass);
    if(GameOverWidget)
    {
        GameOverWidget->SetResult(bWon);
        GameOverWidget->AddToViewport();
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
    }

    if(HUDWidget)
    {
        HUDWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ACoopPlayerController::Client_ShowNotification_Implementation(const FString& Message, float Duration)
{
    if(HUDWidget)
    {
        HUDWidget->ShowNotification(Message, Duration);
    }
}

void ACoopPlayerController::Client_ShowScreenEffect_Implementation(const FLinearColor& Color, float Duration)
{
    if(!ScreenEffectClass) return;
      
    ScreenEffectWidget = CreateWidget<UCoopScreenEffect>(this, ScreenEffectClass);
    if(ScreenEffectWidget)
    {
        ScreenEffectWidget->AddToViewport();
        ScreenEffectWidget->PlayEffect(Color, Duration);
    }
}

void ACoopPlayerController::Server_RestartGame_Implementation()
{
    SetPause(false);

    if(ACoopGameMode* GM = GetWorld()->GetAuthGameMode<ACoopGameMode>())
    {
        GM->RestartGame();
    }
}

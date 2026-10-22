#include "CoopPlayerController.h"
#include "CoopHUD.h"
#include "CoopMainMenu.h"
#include "CoopGameOverScreen.h"
#include "CoopScreenEffect.h"
#include "CoopPauseMenu.h"
#include "CoopGameMode.h"
#include "CoopGameState.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"

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
    ACoopGameState* GS = GetWorld()->GetGameState<ACoopGameState>();
    if (!GS) return;

    GS->bIsPaused = !GS->bIsPaused;

    SetPause(GS->bIsPaused);

    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (ACoopPlayerController* PC = Cast<ACoopPlayerController>(It->Get()))
        {
            PC->Client_SetPause(GS->bIsPaused);
        }
    }
}

void ACoopPlayerController::Client_SetPause_Implementation(bool bPause)
{
    if (bPause && GameOverWidget) return;

    SetPause(bPause);

    if (bPause)
    {
        if (PauseMenuClass && !PauseWidget)
        {
            PauseWidget = CreateWidget<UCoopPauseMenu>(this, PauseMenuClass);
            if (PauseWidget)
            {
                PauseWidget->AddToViewport();
                PauseWidget->SetFocus();
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

    if (PauseWidget)
    {
        PauseWidget->RemoveFromParent();
        PauseWidget = nullptr;
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
      
    if (!ScreenEffectWidget)
    {
        ScreenEffectWidget = CreateWidget<UCoopScreenEffect>(this, ScreenEffectClass);
        if (ScreenEffectWidget) ScreenEffectWidget->AddToViewport();
    }

    if (ScreenEffectWidget)
    {
        ScreenEffectWidget->PlayEffect(Color, Duration);
    }
}

void ACoopPlayerController::Server_RestartGame_Implementation()
{
    if (ACoopGameMode* GM = GetWorld()->GetAuthGameMode<ACoopGameMode>())
    {
        GM->RestartGame();
    }
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPlayerController.h"
#include "CoopHUD.h"
#include "CoopMainMenu.h"
#include "CoopGameOverScreen.h"
#include "CoopScreenEffect.h"
#include "CoopGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ACoopPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    if(!IsLocalController()) return;
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
    if(ACoopGameMode* GM = GetWorld()->GetAuthGameMode<ACoopGameMode>())
    {
        GM->RestartGame();
    }
}

void ACoopPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(HUDWidget)
    {
        HUDWidget->UpdateHUD();
    }
}
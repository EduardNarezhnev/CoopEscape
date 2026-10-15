// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPauseMenu.h"
#include "Components/Button.h"
#include "CoopPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"

void UCoopPauseMenu::NativeConstruct()
{
    Super::NativeConstruct();

    if(Button_Resume)
    {
        Button_Resume->OnClicked.AddDynamic(this, &UCoopPauseMenu::OnResumeClicked);
    }
    if(Button_Restart)
    {
        Button_Restart->OnClicked.AddDynamic(this, &UCoopPauseMenu::OnRestartClicked);
    }
    if(Button_Exit)
    {
        Button_Exit->OnClicked.AddDynamic(this, &UCoopPauseMenu::OnExitClicked);
    }
}

void UCoopPauseMenu::OnResumeClicked()
{
    if(ACoopPlayerController* PC = Cast<ACoopPlayerController>(GetOwningPlayer()))
    {
        PC->TogglePause();
    }
}

void UCoopPauseMenu::OnRestartClicked()
{
    if(ACoopPlayerController* PC = Cast<ACoopPlayerController>(GetOwningPlayer()))
    {
        PC->SetPause(false);
        PC->Server_RestartGame();
    }
}

void UCoopPauseMenu::OnExitClicked()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
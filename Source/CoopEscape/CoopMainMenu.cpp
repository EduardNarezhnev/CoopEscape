// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopMainMenu.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UCoopMainMenu::NativeConstruct()
{
    Super::NativeConstruct();

    if(Button_Host)
    {
        Button_Host->OnClicked.AddDynamic(this, &UCoopMainMenu::OnHostClicked);
    }
    if(Button_Join)
    {
        Button_Join->OnClicked.AddDynamic(this, &UCoopMainMenu::OnJoinClicked);
    }
    if(Button_Exit)
    {
        Button_Exit->OnClicked.AddDynamic(this, &UCoopMainMenu::OnExitClicked);
    }
}

void UCoopMainMenu::OnHostClicked()
{
    UGameplayStatics::OpenLevel(this, FName("Main"), true, "listen");
}

void UCoopMainMenu::OnJoinClicked()
{
    if(!TextBox_IP) return;

    FString IP = TextBox_IP->GetText().ToString();
    if(IP.IsEmpty()) return;

    if(APlayerController* PC = GetOwningPlayer())
    {
        PC->ClientTravel(IP, ETravelType::TRAVEL_Absolute);
    }
}

void UCoopMainMenu::OnExitClicked()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
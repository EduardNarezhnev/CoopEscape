// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPlayerController.h"
#include "CoopHUD.h"
#include "Blueprint/UserWidget.h"

void ACoopPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    if(IsLocalController() && HUDClass)
    {
        HUDWidget = CreateWidget<UCoopHUD>(this, HUDClass);
        if(HUDWidget)
        {
            HUDWidget->AddToViewport();
        }
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
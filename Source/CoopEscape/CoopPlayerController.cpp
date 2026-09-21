// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPlayerController.h"

void ACoopPlayerController::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("CoopPlayerController::BeginPlay() called"));
}
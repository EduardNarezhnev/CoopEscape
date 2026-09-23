// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopKeyPickup.h"
#include "CoopCharacter.h"
#include "CoopPlayerState.h"
#include "CoopGameState.h"

void ACoopKeyPickup::ApplyEffect(ACoopCharacter* Character)
{
    if(!Character || KeyCount <= 0) return;

    if(ACoopPlayerState* PS = Character->GetPlayerState<ACoopPlayerState>())
    {
        PS->PlayerCollectedItems += KeyCount;
    }

    if(ACoopGameState* GS = GetWorld()->GetGameState<ACoopGameState>())
    {
        GS->CollectedItemsCount += KeyCount;
    }

    Destroy();
}
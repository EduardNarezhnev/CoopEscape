// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopGameState.h"
#include "Net/UnrealNetwork.h"

void ACoopGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACoopGameState, CollectedItemsCount);
}
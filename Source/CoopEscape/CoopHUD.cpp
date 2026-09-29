// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopHUD.h"
#include "CoopCharacter.h"
#include "CoopHealthComponent.h"
#include "CoopPlayerState.h"
#include "CoopGameState.h"

void UCoopHUD::UpdateHUD()
{
    ACoopCharacter* Character = Cast<ACoopCharacter>(GetOwningPlayerPawn());
    if(!Character) return;

    if(Character->HealthComponent)
    {
        BP_UpdateHealth(Character->HealthComponent->GetHealthPercent());
    }

    if(ACoopPlayerState* PS = Character->GetPlayerState<ACoopPlayerState>())
    {
        BP_UpdatePersonalKeys(PS->PlayerCollectedItems, 3);
    }

    if(ACoopGameState* GS = GetWorld()->GetGameState<ACoopGameState>())
    {
        BP_UpdateTotalKeys(GS->CollectedItemsCount, 3);
    }
}

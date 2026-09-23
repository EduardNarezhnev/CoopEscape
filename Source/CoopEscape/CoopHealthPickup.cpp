// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopHealthPickup.h"
#include "CoopCharacter.h"
#include "CoopHealthComponent.h"

void ACoopHealthPickup::ApplyEffect(ACoopCharacter* Character)
{
    if(!Character || !Character->HealthComponent) return;
    if(HealAmount <= 0.f) return;

    if(Character->HealthComponent->CurrentHealth < Character->HealthComponent->MaxHealth)
    {
        Character->HealthComponent->Heal(HealAmount);
        Destroy();
    }
}